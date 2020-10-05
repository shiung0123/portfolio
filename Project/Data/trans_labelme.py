"""
Mask R-CNN
Train on the toy Balloon dataset and implement color splash effect.

Copyright (c) 2018 Matterport, Inc.
Licensed under the MIT License (see LICENSE for details)
Written by Waleed Abdulla

------------------------------------------------------------

Usage: import the module (see Jupyter notebooks for examples), or run from
       the command line as such:

    # Train a new model starting from pre-trained COCO weights
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=coco

    # Resume training a model that you had trained earlier
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=last

    # Train a new model starting from ImageNet weights
    python3 balloon.py train --dataset=/path/to/balloon/dataset --weights=imagenet

    # Apply color splash to an image
    python3 balloon.py splash --weights=/path/to/weights/file.h5 --image=<URL or path to file>

    # Apply color splash to video using the last weights you trained
    python3 balloon.py splash --weights=last --video=<URL or path to file>
"""

import os
import glob
import sys
import json

def convert_json(input_dir, rm):
    input_dir = os.path.join(input_dir, '*.json')
    for f in glob.glob(input_dir) :
        j = json.load(open(f))
        
        image_path = j['imagePath']
        image_path_without_ex, image_ex = os.path.splitext(image_path)
        name = os.path.split(image_path_without_ex)[-1]
        size = j['imageHeight'] * j['imageWidth']
        regions = []
        
        shapes = j['shapes']
        for mask in shapes :
            all_points_x = []
            all_points_y = []
            
            for point in mask['points'] :
                all_points_x.append(point[0])
                all_points_y.append(point[1])
            
            region_temp = {
                "shape_attributes": {
					"name": "polygon",
					"label": mask['label'],
					"all_points_x": all_points_x,
					"all_points_y": all_points_y
				},
				"region_attributes": {}
            }
            regions.append(region_temp)
        
        data = {
            j['imagePath']+str(size) : {
                'filename' : j['imagePath'],
                'size' : size,
                'regions' : regions,
                "file_attributes": {}
            }
        }
        
        dataset_out_dir = os.path.split(f)[0]
        out = os.path.join(dataset_out_dir, name+'_vim.json')
        print('Convert ', name)
        fp = open(out, 'w')
        fp.write(json.dumps(data))
        fp.close()
        
        if rm: os.remove(f)
        
if __name__ == '__main__':
    import argparse

    # Parse command line arguments
    parser = argparse.ArgumentParser(
        description='Convert Labelme to Vim')
    parser.add_argument("command",
                        metavar="<command>",
                        help="The input_dir contains labelme_json")
    parser.add_argument('--rm', required=False,
                        metavar="",
                        help='Remove old file')
    args = parser.parse_args()
    
    input_path = args.command
    output_path = args.command

    print("Select_dir: ", input_path)
    
    convert_json(input_path, args.rm)
