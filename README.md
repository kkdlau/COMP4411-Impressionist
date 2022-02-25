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

> W: Whistle, B: Bell
>
> Bold: planned to do
>
> Italic: wanted to do
>
> Underline: wanted to do

- [ ] (1W each) more brushes (M)
  - [ ] pattern brush
  - [ ] pixelize brush
  - [x] random polygon
  - [x] curve brush
  - [ ] random scattered polygon
  - [ ] grayscale brush
- [x] **(1W)** clipped brushes (D)
- [x] **(1W)** cursor on original image (D) [C]
- [x] (1W) swap content and original (D) [C]
- [x] (1B) change color of image (change channel) (M)
- [x] **(1B)** blur brush (M)
- [x] **(1B)** undo (D) [C]
- [x] (1B) Dissolve one image into another (D) [C]
  - [x] (1B1W) original image overlay (D)
    -[x] add a slider to control overlay alpha 
- [x] _(1B1W)_ mural image (load another image without resetting)
- [x] (1B1W) alpha mapping brush (need more look up) (M)
- [x] **(1B1W)** auto paint (M)
- [x] **(1B1W)** gradient according to another image (D)
- [x] _(1B1W)_ edge clipping (need more look up) (D)
- [x] _(2B)_ filter kernel (M)
- [x] _(2B)_ find edges (D)
- [ ] _(2B)_ multiresolution automatic painting technique http://mrl.nyu.edu/projects/npr/painterly/ (M)
- [ ] _(2B)_ stretch and pull image http://mostfungames.com/warp-george-bush.htm
- [ ] _(2B)_ curved brush that follows image gradient http://mrl.nyu.edu/projects/npr/painterly/
- [ ] <u>(4B)</u> thumbnailhttp://www.cs.princeton.edu/~af/cool/webgothic.html (D)
- [ ] <u>(4B)</u> video (M, D)
- [ ] <u>(8B)</u> Bayesian matting (M, D)

## ToDo before submission:
- [ ] alpha-mapped brush can get bitmap that is NOT same dimension as original image    
- [ ] multi resolution should pass the RGBA value to set   
- [ ] warp brush
- [ ] curved brush that follows image gradient  
- [ ] compile with virtual machine  
- [ ] README.md for TA with any special configuration - C++17 (?)  
- [ ] Bell and whistle checklist   