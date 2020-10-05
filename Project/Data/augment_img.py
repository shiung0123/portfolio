"""
1. no empty mask
2. can read multi dot(.) in filename
3. can not read multi slash(/, \) in filename
"""

import os
import glob
import sys
import json
import skimage.io
import skimage.color
import numpy as np
from imgaug import augmenters as iaa

# Root directory of the project
ROOT_DIR = os.path.abspath("./")

# Import Mask RCNN
sys.path.append(ROOT_DIR)  # To find local version of the library


def empty_mask_color(image):
    image = skimage.color.rgb2gray(image)
    image = image > 0.5
    horizontal_indicies = np.where(np.any(image, axis=0))[0]
    if horizontal_indicies.shape[0]: return False
    else: return True

def load_pothole(dataset_dir, dataset_out_dir):
    
    seq_list = []
    seq = iaa.Noop()
    seq_list.append(seq)

    # think about mask augment or not
    mask_aug = True

    for i in range(32):
        seq = iaa.SomeOf((1,4), [
            iaa.Fliplr(1.0),
            iaa.AdditiveGaussianNoise(scale=(0.05*255, 0.1*255)),
            iaa.CoarseDropout(0.2, size_percent=0.3),
            iaa.PerspectiveTransform(scale=(0.075, 0.125))
        ])
        seq = seq.to_deterministic()
        seq_list.append(seq)

    # iaa.Fliplr(1.0)/iaa.Fliplr(0.0)
    # iaa.AdditiveGaussianNoise(scale=0.05*255)/iaa.AdditiveGaussianNoise(scale=0.1*255)/iaa.AdditiveGaussianNoise(scale=0)
    # iaa.CoarseDropout(0.2, size_percent=0.3)
    # iaa.PerspectiveTransform(scale= 0.075 0.1 0.125)

    json_path = os.path.join(dataset_dir, "*.json")
    for f in glob.glob(json_path) : # get a json

        annotations = json.load(open(f))
        annotations = annotations.values()  # don't need the dict keys
        

        for a in annotations: # just need value of dict

            image_path_without_ex, image_ex = os.path.splitext(a['filename'])
            name = os.path.split(image_path_without_ex)[-1]

            for aug_index in range(len(seq_list)): # get a aug
                image_path = os.path.join(dataset_dir, a['filename'])
                image_name = name + str(aug_index) + ".jpg"
                image_out_path = os.path.join(dataset_out_dir, image_name)
                
                if not os.path.exists(image_path): 
                    print(image_path, "not exist")
                else: 
                    print(image_path, "success")

                    image = skimage.io.imread(image_path)
                    image = seq_list[aug_index].augment_image(image)
                    skimage.io.imsave( image_out_path, image )

                    masks = []
                    mask = a['mask']
                    for mask_index in range(len(mask)):

                        mask_path = os.path.join(dataset_dir, mask[mask_index]['mask_path'])
                        mask_name = name + str(aug_index)+ "_mask" + str(mask_index) + ".jpg"
                        mask_out_path = os.path.join(dataset_out_dir, mask_name)

                        if not os.path.exists(mask_path): 
                            print(mask_path, "not exist")
                        else: 
                            print(mask_path, "success")

                            image = skimage.io.imread(mask_path)
                            # mask augment or not
                            if mask_aug: image = seq_list[aug_index].augment_image(image)

                            if not empty_mask_color(image) :
                                skimage.io.imsave( mask_out_path, image )
                                mask_temp = {
                                    "label": mask[mask_index]['label'],
                                    "mask_path": mask_name
                                }
                                masks.append(mask_temp)

                    data = {
                        image_name+str(a['size']) : {
                            'filename' : image_name,
                            'size' : a['size'],
                            'mask' : masks,
                            "file_attributes": {}
                        }
                    }

                    out = os.path.join(dataset_out_dir, name + str(aug_index) + '.json')
                    fp = open(out, 'w')
                    fp.write(json.dumps(data))
                    fp.close()

if __name__ == '__main__':
    import argparse

    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Train Mask R-CNN to detect pothole.')
    parser.add_argument("command",
                        metavar="<command>",
                        help="'train' or 'splash'")
    parser.add_argument('--dataset', required=False,
                        metavar="/path/to/pothole/dataset/",
                        help='Directory of the Pothole dataset')
    parser.add_argument('--dataset_out', required=False,
                        metavar="/path/to/pothole/dataset/",
                        help='Directory of the Pothole dataset')
    args = parser.parse_args()
    
    # Validate arguments
    if args.command == "augment":
        assert args.dataset, "Argument --dataset is required for training"
    elif args.command == "test": sys.exit(0)

    print("Dataset: ", args.dataset)

    # Train or evaluate
    if args.command == "augment":
        load_pothole(args.dataset, args.dataset_out)

