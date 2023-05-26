import numpy as np

import matplotlib.pyplot as plt

from pathlib import Path

from PIL import Image

from typing import Tuple

import torch
import torch.nn.functional as F
from torch.utils.data import Dataset, DataLoader
from torchvision import transforms, utils

from torch.nn import Module
from torch.nn import Sequential
from torch.nn import Conv2d, Dropout2d, MaxPool2d, ReLU, UpsamplingNearest2d

from tqdm import tqdm_notebook as tqdm


from torchsummary import summary


batch_size = 4
epochs = 5
learning_rate = 0.001

number_of_classes = 2

print('Using CUDA' if torch.cuda.is_available() else 'Using CPU')

device = torch.device('cuda') if torch.cuda.is_available() else torch.device('cpu')

lines_detector_path = Path('lines-segmentation')

lines_detector_train_images_path = lines_detector_path / "train" / "images"
lines_detector_train_masks_path = lines_detector_path / "train" / "masks"

lines_detector_valid_images_path = lines_detector_path / "valid" / "images"
lines_detector_valid_masks_path = lines_detector_path / "valid" / "masks"

lines_detector_test_images_path = lines_detector_path / "test" / "images"
lines_detector_test_masks_path = lines_detector_path / "test" / "masks"

model_save_path = Path("lines-segmentation/lines_segmentation.ckpt")

img_to_array = lambda image: np.asarray(image)
array_to_img = lambda array: Image.fromarray(array)


def load_images_and_masks_in_path(images_path: Path, masks_path: Path) -> Tuple[np.ndarray, np.ndarray]:
    x = []
    y = []

    sorted_image_names = sorted(images_path.glob("*.png"))
    sorted_mask_names = sorted(masks_path.glob("*.png"))
    for image_file_name, mask_file_name in tqdm(zip(sorted_image_names, sorted_mask_names)):
        image = img_to_array(Image.open(image_file_name))
        mask = img_to_array(Image.open(mask_file_name))

        x.append(image)
        y.append(mask)

    return np.array(x), np.array(y)


train_images, train_masks = load_images_and_masks_in_path(lines_detector_train_images_path, lines_detector_train_masks_path)
valid_images, valid_masks = load_images_and_masks_in_path(lines_detector_valid_images_path, lines_detector_valid_masks_path)
test_images, test_masks = load_images_and_masks_in_path(lines_detector_test_images_path, lines_detector_test_masks_path)


class UNetMini(Module):

    def __init__(self, num_classes):
        super().__init__()

        # TODO: Develop the Unet model for text segmentation.
        # To implement Unet, you should create an encoder, a decoder and a connecting path between them.
        # Write code here

    def forward(self, x):
        # TODO: Develop a forward method for feeding data
        # Write code here
        print('')


def get_transformations(train=False):
    transformations = [transforms.ToTensor()]

    if train:
        pass

    return transforms.Compose(transformations)

def main():
    print('Started')


if __name__ == "main":
    main()