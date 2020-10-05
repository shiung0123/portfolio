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
from mrcnn import utils

# Root directory of the project
ROOT_DIR = os.path.abspath("./")

# Import Mask RCNN
sys.path.append(ROOT_DIR)  # To find local version of the library

IMAGE_MIN_DIM = 256
IMAGE_MAX_DIM = 256

def load_pothole(dataset_dir, dataset_out_dir):
    
    json_path = os.path.join(dataset_dir, "*.json")
    for f in glob.glob(json_path) : # get a json

        annotations = json.load(open(f))
        annotations = annotations.values()  # don't need the dict keys
        annotations = [a for a in annotations if a['mask']]
        
        for a in annotations: # just need value of dict

            image_path_without_ex, image_ex = os.path.splitext(a['filename'])
            name = os.path.split(image_path_without_ex)[-1]

            img_size = 0
            del_list = []

            # Resize Image

            image_path = os.path.join(dataset_dir, a['filename'])
            image_name = a['filename']
            image_out_path = os.path.join(dataset_out_dir, image_name)
                
            if not os.path.exists(image_path):
                img_size = -1
                print(image_path, "not exist")
            else: 
                print(image_path, "success")
                image = skimage.io.imread(image_path)
                image, _, _, _, _ = utils.resize_image(
                    image,
                    min_dim=IMAGE_MIN_DIM,
                    min_scale=0,
                    max_dim=IMAGE_MAX_DIM,
                    mode="square")
                height, width = image.shape[:2]
                img_size = height * width
                skimage.io.imsave(image_out_path, image)
                del_list.append(image_out_path)

            # Resize Mask

            for m in a['mask'] :

                mask_path = os.path.join(dataset_dir, m['mask_path'])
                mask_name = m['mask_path']
                mask_out_path = os.path.join(dataset_out_dir, mask_name)

                if not os.path.exists(mask_path):
                    img_size = -1
                    print(mask_path, "not exist")
                else: 
                    print(mask_path, "success")
                    mask = skimage.io.imread(mask_path)
                    mask, _, _, _, _ = utils.resize_image(
                        mask,
                        min_dim=IMAGE_MIN_DIM,
                        min_scale=0,
                        max_dim=IMAGE_MAX_DIM,
                        mode="square")
                    skimage.io.imsave(mask_out_path, mask)
                    del_list.append(mask_out_path)

            if img_size != -1 :
                data = {
                    image_name+str(img_size) : {
                        'filename' : image_name,
                        'size' : img_size,
                        'mask' : a['mask'],
                        "file_attributes": {}
                    }
                }

                out = os.path.join(dataset_out_dir, name + '.json')
                fp = open(out, 'w')
                fp.write(json.dumps(data))
                fp.close()
            else :
                for f in del_list : os.remove(f)

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
    if args.command == "resize":
        assert args.dataset, "Argument --dataset is required for training"
        assert IMAGE_MIN_DIM % 64 == 0, "IMAGE_MIN_DIM must divide by 2**6"
        assert IMAGE_MAX_DIM % 64 == 0, "IMAGE_MAX_DIM must divide by 2**6"
    elif args.command == "test": sys.exit(0)

    print("Dataset: ", args.dataset)

    # Train or evaluate
    if args.command == "resize":
        load_pothole(args.dataset, args.dataset_out)

