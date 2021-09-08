# SSIM

## Overview

This is a utility for measuring the SSIM between two png images.

It has two versions of the metric, both from libvpx. There is a third
implementation that is not currently built but used to be hosted
at http://mehdi.rabah.free.fr/SSIM/

## Usage

ssim [type] [resolution] [file1] [file2]

## Example

ssim bgra 1280x720 a.bin b.bin
ssim png a.png b.png
