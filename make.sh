#!/bin/bash

clear
make clean
make ebc2ebu

./ebc2ebu sample_images/gradient.ebc sample_images/out.ebu

