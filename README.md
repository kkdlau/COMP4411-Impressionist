# COMP4411 - Impressionist

## Basic Requirment

- Implement 5 different brush types
  - [x] single line [C]
  - [x] scattered lines [C]
    - scattered brushes should sample from each location they color individually, not just use a single color for each splotch. [C]
  - [x] (filled) circles [C]
  - [x] scattered (filled) circles - 3 random circles [C] 
- sliders to control various brush attributes [C]
  - [x] line thickness [C]
  - [x] brush angle [C
- control the brush direction: The stroke direction should be controlled **four different ways**. You can use a radio box to allow the user to select which method to use.
  - [x] slider value [C]
  - [x] right mouse button to drag out a direction line [C]
  - [x] using the direction of the cursor movement [C]
  - [x] using directions that are perpendicular to the gradient of the image [C]
- [x] Allow the user to change the opacity (alpha value) of the brush stroke [C]
- [x] An alpha value slider should be added to the controls window. [C]
- [x] Implement the UI logic when the user selects a brush. - move from impressionistUI to ImpBrush [C]
  - By overriding `select()` and `disselect()`. 
## Bonus

W per new brush						yes/no
W border clipping					yes/no
W (red) marker						yes/no
W swap							yes/no
B RGB scale						yes/no
B blur/sharp brush					yes/no
B 1 level undo						yes/no
B dissolve						yes/no 
BW fade in/out input on output canvas			yes/no
BW mural (blending)					yes/no
BW alpha brush						yes/no
BW automatic paint whole image				yes/no
BW gradient from user-specified image			yes/no
BW edge clipping					yes/no
2B user-specified kernel				yes/no
2B edge detection + user-specified edge clipping	yes/no
2B multiresolution					yes/no
2B warp brush						yes/no
2B curved brush following image gradient		yes/no
4B mosaic						yes/no
4B impressionist video with temporal coherency		yes/no
8B Bayesian matting					yes/no

## ToDo before submission:
- [ ] marks with alpha value < 1 disappear when window resized
- [ ] empty spaces in autopaint and multirespaint - check if shuffle is taking away values? or for_each?? 
    because if repeat a few times, the holes are covered -> dirty fix is to call autopaint a few times ><
- [ ] warp brush
- [ ] curved brush that follows image gradient  
- [ ] compile with virtual machine  
- [ ] README.md for TA with any special configuration - C++17 (?)  
- [ ] Bell and whistle checklist