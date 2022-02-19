# COMP4411 - Impressionist

## Basic Requirment

- Implement 5 different brush types
  - [x] single line
  - [x] scattered lines
    - scattered brushes should sample from each location they color individually, not just use a single color for each splotch.
  - [x] (filled) circles
  - [x] scattered (filled) circles - 3 random circles 
- sliders to control various brush attributes
  - [x] line thickness
  - [x] brush angle
- control the brush direction: The stroke direction should be controlled **four different ways**. You can use a radio box to allow the user to select which method to use.
  - [x] slider value
  - [x] right mouse button to drag out a direction line
  - [x] using the direction of the cursor movement
  - [x] using directions that are perpendicular to the gradient of the image
- [x] Allow the user to change the opacity (alpha value) of the brush stroke
- [x] An alpha value slider should be added to the controls window.
- [x] Implement the UI logic when the user selects a brush. - move from impressionistUI to ImpBrush
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
- [x] **(1W)** clipped brushes (D)
- [x] **(1W)** cursor on original image (D)
- [x] (1W) swap content and original (D)
- [x] (1B) change color of image (change channel) (M)
- [x] **(1B)** blur brush (M)
- [x] **(1B)** undo (D)
- [x] (1B) Dissolve one image into another (D)
  - [x] (1B1W) original image overlay (D)
    -[ ] add a slider to control overlay alpha 
- [x] _(1B1W)_ mural image (load another image without resetting)
- [ ] (1B1W) alpha mapping brush (need more look up) (M)
- [x] **(1B1W)** auto paint (M)
- [ ] **(1B1W)** gradient according to another image (D)
- [x] _(1B1W)_ edge clipping (need more look up) (D)
- [ ] _(2B)_ filter kernel (M)
  - [ ] need arbitrary size
  - [ ] apply button for do whole convolution
  - [ ] normalize
- [ ] _(2B)_ find edges (D)
- [ ] _(2B)_ multiresolution automatic painting technique http://mrl.nyu.edu/projects/npr/painterly/ (M)
- [ ] _(2B)_ stretch and pull image http://mostfungames.com/warp-george-bush.htm
- [ ] _(2B)_ curved brush that follows image gradient http://mrl.nyu.edu/projects/npr/painterly/
- [ ] <u>(4B)</u> thumbnailhttp://www.cs.princeton.edu/~af/cool/webgothic.html (D)
- [ ] <u>(4B)</u> video (M, D)
- [ ] <u>(8B)</u> Bayesian matting (M, D)