import torch
import torch.utils.data

import pandas as pd
from PIL import Image


class ImgDataSet(torch.utils.data.Dataset):
    def __init__(self, csvname, transformer=None):
        self.csvname = csvname
        self.transformer = transformer

        self.df = pd.read_csv(csvname)

    def __len__(self):
        return self.df.shape[0]

    def __getitem__(self, idx):
        imgpath = self.df.loc[idx, "path"]
        img = Image.open(imgpath)

        label = self.df.loc[idx, "label"]

        if self.transformer is not None:
            img = self.transformer(img)

        return img, label


def get_data_loader(train_transformer, valid_transformer, batch_size=32):
    train_dataset = ImgDataSet("train.csv", transformer=train_transformer)
    valid_dataset = ImgDataSet("valid.csv", transformer=valid_transformer)

    # Note: You can change the batch_size below depends on your hardware
    # If you are using a CPU or Laptop, the bacth size should be below 50
    # IF you are using a GPU, the batch size can be 100 or even 1000
    trainloader = torch.utils.data.DataLoader(train_dataset,
                                              batch_size=batch_size, shuffle=True)
    validloader = torch.utils.data.DataLoader(valid_dataset,
                                              batch_size=batch_size, shuffle=False)

    return trainloader, validloader
