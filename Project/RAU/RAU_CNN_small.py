import tensorflow as tf
import numpy as np
from datetime import datetime
import os
import glob
import json
import cv2

#####################################################################################
#                                    config                                         #
#####################################################################################

PRINT = True

config = tf.ConfigProto()
config.gpu_options.allow_growth = True
session = tf.Session(config=config)

STEPS = 1000
BATCH_SIZE = 3
IMAGE_SIZE_HEIGHT = 480
IMAGE_SIZE_WIDTH = 240
CLASS_NAME = ['bg', 'pothole', 'pothole_ww']
LABEL_NUM = 3
LEARN_RATE = 1e-4

#####################################################################################
#                                      data                                         #
#####################################################################################

class myData:
    def __init__(self, path):
        # nothing
        self.path = path
    
    def extract_bboxes(self, mask):
        mask = mask > 50
        horizontal_indicies = np.where(np.any(mask, axis=0))[0]
        vertical_indicies = np.where(np.any(mask, axis=1))[0]
        if horizontal_indicies.shape[0]:
            x1, x2 = horizontal_indicies[[0, -1]]
            y1, y2 = vertical_indicies[[0, -1]]
            # x2 and y2 should not be part of the box. Increment by 1.
            x2 += 1
            y2 += 1
        else:
            # No mask for this instance. Might happen due to
            # resizing or cropping. Set bbox to zeros
            x1, x2, y1, y2 = 0, 0, 0, 0

        return x1, y1, x2, y2

    def read_data(self):
        global IMAGE_SIZE_HEIGHT, IMAGE_SIZE_WIDTH, LABEL_NUM, CLASS_NAME, PRINT
        image = []
        label = []

        dataset_dir = self.path
        json_path = os.path.join(dataset_dir, "*.json")
        for f in glob.glob(json_path):
            annotations = json.load(open(f))
            annotations = list(annotations.values())
            
            for a in annotations:
                image_path = os.path.join(dataset_dir, a['filename'])

                if not os.path.exists(image_path): 
                    print(image_path, "not exist")
                else: 
                    if PRINT: print(image_path, "success")
                    image_ori = cv2.imread(image_path)

                    if a['mask']:
                        for m in a['mask']:
                            mask_path = os.path.join(dataset_dir, m['mask_path'])

                            if not os.path.exists(mask_path): 
                                print(mask_path, "not exist")
                            else: 
                                if PRINT: print(mask_path, "success")
                                mask = cv2.imread(mask_path, 0) # gray
                                x1, y1, x2, y2 = self.extract_bboxes(mask)

                                image_temp = image_ori[:, x1:x2] # cut img
                                image_temp = cv2.resize(image_temp, (IMAGE_SIZE_WIDTH, IMAGE_SIZE_HEIGHT)) # w, h <- special ( normal -> h, w )
                                image.append(image_temp)

                                label_temp = np.zeros([LABEL_NUM], dtype=np.int32)
                                label_temp[CLASS_NAME.index(m['label'])] = 1
                                label.append(label_temp)
                    else:
                        image_temp = cv2.resize(image_ori, (IMAGE_SIZE_WIDTH, IMAGE_SIZE_HEIGHT)) # w, h <- special ( normal -> h, w )
                        image.append(image_temp)

                        label_temp = np.zeros([LABEL_NUM], dtype=np.int32)
                        label_temp[0] = 1
                        label.append(label_temp)

        return np.array(image), np.array(label)

#####################################################################################
#                                     model                                         #
#####################################################################################

class myCNN:
    def __init__(self, path, model_type):
        global IMAGE_SIZE_HEIGHT, IMAGE_SIZE_WIDTH, LABEL_NUM

        self.sess = tf.Session()

        print("Start Building Model...")

        # Input images & labels
        self.x  = tf.placeholder(tf.float32, shape = [None, IMAGE_SIZE_HEIGHT, IMAGE_SIZE_WIDTH, 3], name="input_image")
        self.y_label = tf.placeholder(tf.int32, shape = [None, LABEL_NUM], name="label")

        if model_type == "v1": self.build_model()
        elif model_type == "v2": self.build_model_2v()

        print("Building Model Complete.")

        print("Start Reading Data...")

        dataset = myData(path)
        self.images, self.labels = dataset.read_data()
        self.size = len(self.images)
        self.cur = 0

        print("Reading Data Complete.")

    def next_batch(self, next_size, shuffle=False):
        if self.cur == 0:
            i = np.arange(len(self.images))
            if shuffle: np.random.shuffle(i)
            self.shuff = i

        index = self.shuff[self.cur : self.cur + next_size]
        self.cur += next_size
        if self.cur >= self.size: self.cur = 0

        return self.images[index], self.labels[index]

    def weight_variable(self, shape, mean=0, stddev=1):
        init = tf.truncated_normal(shape, mean=mean, stddev=stddev)
        return tf.Variable(init)

    def bias_variable(self, shape, mean=0, stddev=1):
        init = tf.truncated_normal(shape, mean=mean, stddev=stddev)
        return tf.Variable(init)

    def conv2d(self, x, w, strides=[1, 1, 1, 1]):
        # (b, h, w, in_ch) -> (b, h, w, out_ch)
        return tf.nn.conv2d(x, w, strides=strides, padding='SAME')

    def max_pool_2x2(self, x, ksize=[1, 2, 2, 1], strides=[1, 2, 2, 1]):
        # (b, h, w, in_ch) -> (b, h, w, in_ch/2)
        return tf.nn.max_pool(x, ksize=ksize, strides=strides, padding='SAME')

    def RAU(self, x, n, increase_deep):

        h = x.shape[1].value
        w = x.shape[2].value
        deep = x.shape[3].value
        x_out = x
        # 切成n塊、平均 (None, 15, 15, deep)
        x_pool = tf.nn.avg_pool(x, ksize=[1, h/n, h/(n*2), 1], strides=[1, h/n, h/(n*2), 1], padding="SAME")
        
        if (h%n != 0): n = n+1
        for k in range(n):
            # 拿出一塊的橫切面 (None, 1, 15, deep)
            x_slice = x_pool[:, k:k+1, :, :]
            # 將橫切面以二向插值法補回原大小 (None, h, w, deep)
            x_slice_reshape = tf.image.resize_images(x_slice, [h,w], 1)
            # 藉由相減尋找圖片中相似之處
            x_abs = tf.abs(x - x_slice_reshape)

            if increase_deep == deep :
                x_out = tf.concat([x_out, x_abs], axis=3)
            else :
                w1 = self.weight_variable([1, 1, deep, increase_deep])
                b1 = self.bias_variable([increase_deep])
                conv1 = self.conv2d(x_abs, w1) + b1
                x_out = tf.concat([x_out, conv1], axis=3)

        return x_out

    def build_model(self):
        global LABEL_NUM

        # conv_1 (480, 240, 3)/(240, 120, 16)
        self.w_conv1 = self.weight_variable([5, 5, 3, 16])
        self.b_conv1 = self.bias_variable([16])
        self.y_conv1 = self.conv2d(self.x, self.w_conv1) + self.b_conv1
        self.relu_1 = tf.nn.relu(self.y_conv1)
        self.pool_1 = self.max_pool_2x2(self.relu_1)

        # conv_2 (240, 120, 16)/(120, 60, 32)
        self.w_conv2 = self.weight_variable([5, 5, 16, 32])
        self.b_conv2 = self.bias_variable([32])
        self.y_conv2 = self.conv2d(self.pool_1, self.w_conv2) + self.b_conv2
        self.relu_2 = tf.nn.relu(self.y_conv2)
        self.pool_2 = self.max_pool_2x2(self.relu_2)

        # rau_1 (120, 60, 32)/(120, 60, 15*6+32)
        self.rau_1 = self.RAU(self.pool_2, 15, 6) 

        # conv_3 (120, 60, 122)/(60, 30, 256)
        self.w_conv3 = self.weight_variable([5, 5, 122, 256])
        self.b_conv3 = self.bias_variable([256])
        self.y_conv3 = self.conv2d(self.rau_1, self.w_conv3) + self.b_conv3
        self.relu_3 = tf.nn.relu(self.y_conv3)
        self.pool_3 = self.max_pool_2x2(self.relu_3)

        # rau_2 (120, 60, 256)/(120, 60, 15*12+256)
        self.rau_2 = self.RAU(self.pool_3, 15, 12) 

        # conv_4 (60, 30, 436)/(30, 15, 512)
        self.w_conv4 = self.weight_variable([5, 5, 436, 512])
        self.b_conv4 = self.bias_variable([512])
        self.y_conv4 = self.conv2d(self.rau_2, self.w_conv4) + self.b_conv4
        self.relu_4 = tf.nn.relu(self.y_conv4)
        self.pool_4 = self.max_pool_2x2(self.relu_4)

        # fc_1 (30, 15, 512)/(128)
        self.pool_4_flat = tf.reshape(self.pool_4, [-1, 30*15*512])
        self.w_fc1 = self.weight_variable([30*15*512, 128])
        self.b_fc1 = self.bias_variable([128])
        self.y_fc1 = tf.matmul(self.pool_4_flat, self.w_fc1) + self.b_fc1
        self.relu_5 = tf.nn.relu(self.y_fc1)

        # fc_2 (128)/(3)
        self.w_fc2 = self.weight_variable([128, LABEL_NUM])
        self.b_fc2 = self.bias_variable([LABEL_NUM])
        self.y = tf.matmul(self.relu_5, self.w_fc2) + self.b_fc2

    def RAU_2v(self, x, n, increase_deep):

        h = x.shape[1].value
        w = x.shape[2].value
        deep = x.shape[3].value

        x_all_abs = x[:, :, :, 0:0]
        x_all_out = x[:, :, :, 0:0]

        # 切成n塊、平均 (None, 15, 15, deep)
        x_pool = tf.nn.avg_pool(x, ksize=[1, h/n, h/(n*2), 1], strides=[1, h/n, h/(n*2), 1], padding="SAME")
        
        if (h%n != 0): n = n+1
        for k in range(n):
            # 拿出一塊的橫切面 (None, 1, 15, deep)
            x_slice = x_pool[:, k:k+1, :, :]
            # 將橫切面以二向插值法補回原大小 (None, h, w, deep)
            x_slice_reshape = tf.image.resize_images(x_slice, [h,w], 1)
            # 藉由相減尋找圖片中相似之處
            x_abs = tf.abs(x - x_slice_reshape)
            x_all_abs = tf.concat([x_all_abs, x_abs], axis=3)

            if not increase_deep == deep:
                w1 = self.weight_variable([1, 1, deep, increase_deep])
                b1 = self.bias_variable([increase_deep])
                conv1 = self.conv2d(x_abs, w1) + b1
                x_all_out = tf.concat([x_all_out, conv1], axis=3)
        
        if not increase_deep == deep:
            for d in range(deep):
                x_temp = x[:, :, :, 0:0]
                for k in range(n):
                    x_slice = x_all_abs[:, :, :, k*deep+d:k*deep+d+1]
                    x_temp = tf.concat([x_temp, x_slice], axis=3)

                w1 = self.weight_variable([1, 1, n, increase_deep])
                b1 = self.bias_variable([increase_deep])
                conv1 = self.conv2d(x_temp, w1) + b1
                x_all_out = tf.concat([x_all_out, conv1], axis=3)
        else :
            x_all_out = x_all_abs

        x_out = tf.concat([x, x_all_out], axis=3)
        return x_out

    def build_model_2v(self):
        global LABEL_NUM

        # conv_1 (480, 240, 3)/(240, 120, 16)
        self.w_conv1 = self.weight_variable([5, 5, 3, 16])
        self.b_conv1 = self.bias_variable([16])
        self.y_conv1 = self.conv2d(self.x, self.w_conv1) + self.b_conv1
        self.relu_1 = tf.nn.relu(self.y_conv1)
        self.pool_1 = self.max_pool_2x2(self.relu_1)

        # conv_2 (240, 120, 16)/(120, 60, 32)
        self.w_conv2 = self.weight_variable([5, 5, 16, 32])
        self.b_conv2 = self.bias_variable([32])
        self.y_conv2 = self.conv2d(self.pool_1, self.w_conv2) + self.b_conv2
        self.relu_2 = tf.nn.relu(self.y_conv2)
        self.pool_2 = self.max_pool_2x2(self.relu_2)

        # rau_1 (120, 60, 32)/(120, 60, (15+32)*2+32)
        self.rau_1 = self.RAU_2v(self.pool_2, 15, 2) 

        # conv_3 (120, 60, 126)/(60, 30, 64)
        self.w_conv3 = self.weight_variable([5, 5, 126, 64])
        self.b_conv3 = self.bias_variable([64])
        self.y_conv3 = self.conv2d(self.rau_1, self.w_conv3) + self.b_conv3
        self.relu_3 = tf.nn.relu(self.y_conv3)
        self.pool_3 = self.max_pool_2x2(self.relu_3)

        # rau_2 (120, 60, 64)/(120, 60, (15+64)*2+64)
        self.rau_2 = self.RAU_2v(self.pool_3, 15, 2) 

        # conv_4 (60, 30, 222)/(30, 15, 128)
        self.w_conv4 = self.weight_variable([5, 5, 222, 128])
        self.b_conv4 = self.bias_variable([128])
        self.y_conv4 = self.conv2d(self.rau_2, self.w_conv4) + self.b_conv4
        self.relu_4 = tf.nn.relu(self.y_conv4)
        self.pool_4 = self.max_pool_2x2(self.relu_4)

        # fc_1 (30, 15, 128)/(64)
        self.pool_4_flat = tf.reshape(self.pool_4, [-1, 30*15*128])
        self.w_fc1 = self.weight_variable([30*15*128, 64])
        self.b_fc1 = self.bias_variable([64])
        self.y_fc1 = tf.matmul(self.pool_4_flat, self.w_fc1) + self.b_fc1
        self.relu_5 = tf.nn.relu(self.y_fc1)

        # fc_2 (64)/(3)
        self.w_fc2 = self.weight_variable([64, LABEL_NUM])
        self.b_fc2 = self.bias_variable([LABEL_NUM])
        self.y = tf.matmul(self.relu_5, self.w_fc2) + self.b_fc2

    def train(self, save_path):
        global LEARN_RATE, BATCH_SIZE, STEPS, PRINT

        with tf.name_scope('entropy'):
            cross_entropy = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits_v2(labels=self.y_label, logits=self.y))
            tf.summary.scalar('cross_entropy', cross_entropy)
            train_step = tf.train.AdamOptimizer(LEARN_RATE).minimize(cross_entropy)

        with tf.name_scope('accuracy'):
            correct_prediction = tf.equal(tf.argmax(self.y_label, 1), tf.argmax(self.y, 1))
            accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))
            tf.summary.scalar('accuracy', accuracy)

        merged = tf.summary.merge_all()
        filewriter = tf.summary.FileWriter(save_path+'/Tensorboard',tf.Session().graph)

        self.sess.run(tf.global_variables_initializer())
        self.sess.run(tf.local_variables_initializer())
        print("Start Training...")

        sum = 0
        for i in range(1,STEPS+1):
            # read next batch data
            x_data, y_data = self.next_batch(BATCH_SIZE, shuffle=True)
            # train a step
            self.sess.run(train_step, feed_dict={ self.x: x_data, self.y_label: y_data })

            # record information every 10 steps
            if i % 10 == 0:
                # get train_accuracy
                train_accuracy = self.sess.run(accuracy, feed_dict={ self.x: x_data, self.y_label: y_data })
                sum += train_accuracy
                # get & merge value
                result = self.sess.run(merged, { self.x: x_data, self.y_label: y_data })
                # write value in tensorboard
                filewriter.add_summary(result, i)
                if PRINT: print('step %d, training accuracy %g' % (i, train_accuracy))
        
            # save model every 100 steps
            if i % 100 == 0:
                saver = tf.train.Saver()
                saver.save(self.sess, save_path + "/cnn-model" + str(i) + ".ckpt")
                print('avg accuracy %g' % (sum / (i/10+1)))
        
        print("Training completed.")

    def evaluate(self, model_path):
        global CLASS_NAME, PRINT

        with tf.name_scope('pridict'):
            ans = tf.argmax(tf.nn.softmax(self.y), 1)

        saver = tf.train.Saver()
        saver.restore(self.sess, model_path)
        print("Model restored from file: %s" % model_path)

        print("Start Testing...")

        sum = 0
        for i in range(self.size):
            # read next batch data
            x_data, y_data = self.next_batch(1)

            # only one label data, and one search result, and one item
            ans_label = np.where(y_data[0]==1)[0]
            ans_label = CLASS_NAME[ans_label[0]]

            ans_predict = self.sess.run(ans, feed_dict={self.x: x_data})
            ans_predict = CLASS_NAME[ans_predict[0]]

            if ans_label == ans_predict: sum += 1
            if PRINT: print('label ans = %s, predict ans = %s' % (ans_label, ans_predict))

        print('accuracy %g' % (sum/self.size))

if __name__ == '__main__':
    import argparse

    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Train CNN with RAU to detect pothole or puddle.')
    parser.add_argument("command",
                        metavar="<command>",
                        help="'train'")
    parser.add_argument('--dataset', required=False,
                        metavar="/path/to/dataset",
                        help='Directory of the dataset')
    parser.add_argument('--model', required=False,
                        metavar="v1 or v2", default="v1",
                        help='Select model with RAU version')
    parser.add_argument('--save_path', required=False,
                        metavar="/path/to/save_path", default="RAU_logs",
                        help='Directory of the result path')
    parser.add_argument('--restore_path', required=False,
                        metavar="/path/to/model.ckpt",
                        help="Path of the restore model")
    parser.add_argument('--print', required=False,
                        metavar="True or False", default=True,
                        help="Whether to print the message")
    args = parser.parse_args()

    # Validate arguments
    if args.command == "train":
        assert args.dataset, "Argument --dataset is required for training"
        assert args.model, "Argument --dataset is required for training"
    elif args.command == "evaluate":
        assert args.dataset, "Argument --dataset is required for testing"
        assert args.model, "Argument --dataset is required for testing"
        assert args.restore_path, "Argument --restore_path is required for testing"

    PRINT = args.print
    if args.command == "train":
        model = myCNN(args.dataset, args.model)
        model.train(args.save_path + '/' + datetime.now().strftime('%Y%m%d%H%M%S'))
    elif args.command == "evaluate":
        model = myCNN(args.dataset, args.model)
        model.evaluate(args.restore_path)