## Overview

The program processes bitmap image files as specified in `input.txt`, where each image goes through multiple transformation tasks. A `pixel` structure is defined with three attributes: `red`, `green`, and `blue`.

---

## Task 1: Black & White Conversion

- Open the binary image file and read the two headers and pixel matrix.
- Compute row padding using `getPadding(width)`. Each row's byte size (without padding) is `width * 3`.
- Implemented `goBlackWhite()` to iterate through each pixel and apply the grayscale conversion formula.
- The grayscale image is stored in a new matrix and written to a new binary file.
- Filename generation is done using `getOutputImageName(originalName, extension)`:
  - Splits the name using `"."`, inserts the new extension between name and suffix.

---

## Task 2: Square Image Padding

- Open input/output binary files and write the unchanged file header to the output.
- Three main cases handled using `if/else`:
  1. **Height > Width**:
     - Adjust width to match height.
     - Compute pixel rows to add on top and bottom (white padding).
     - Write new info header and padded matrix.
  2. **Width > Height**:
     - Adjust height similarly and add vertical white padding.
  3. **Equal Dimensions**:
     - Copy image pixel-by-pixel as no transformation is needed.
- Each of the above handles both even and odd differences between width and height separately.

---

## Task 3: Filter Application

- Read reversed pixel matrix, filter size, and filter elements.
- For each pixel, apply the filter to the centered neighborhood (skipping out-of-bounds neighbors).
- Compute new RGB values, clamp them to [0, 255] range.
- Write the updated image (headers + new pixel matrix) to the output binary file.
