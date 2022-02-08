# COMP4411 - Impressionist

## Basic Requirment

- Implement 5 different brush types
  - [x] single line
  - [x] scattered lines
    - scattered brushes should sample from each location they color individually, not just use a single color for each splotch.
  - [x] (filled) circles
  - [x] scattered (filled) circles
- sliders to control various brush attributes
  - [x] line thickness
  - [x] brush angle
- control the brush direction: The stroke direction should be controlled **four different ways**. You can use a radio box to allow the user to select which method to use.
  - [x] slider value
  - [ ] right mouse button to drag out a direction line
  - [ ] using the direction of the cursor movement
  - [ ] using directions that are perpendicular to the gradient of the image
- [ ] Allow the user to change the opacity (alpha value) of the brush stroke
- [x] An alpha value slider should be added to the controls window.
## Bonus

> W: Whistle, B: Bell
>
> Bold: planned to do
>
> Italic: wanted to do
>
> Underline: wanted to do

- [ ] (1W each) more brushes
  - [ ] pattern brush
  - [ ] pixelize brush
  - [ ] random polygon
  - [ ] random scattered polygon
  - [ ] clone brush
- [ ] **(1W)** clipped bushes
- [ ] **(1W)** cursor on original image
- [ ] (1W) swap content and original
- [ ] (1B) change color of image (change channel)
- [ ] **(1B)** blur brush and sharpen brush (wait for kernel brush)
- [ ] **(1B)** undo
- [ ] (1B) Dissolve one image into another
  - [ ] (1B1W) original image overlay
- [ ] _(1B1W)_ mural image (load another image without resetting)
- [ ] (1B1W) alpha mapping brush
- [ ] **(1B1W)** auto paint
  - [ ] button
  - [ ] whole image
  - [ ] alter attributes randomly
  - [ ] random sequence
- [ ] **(1B1W)** gradient according to another image
- [ ] _(1B1W)_ edge clipping
- [ ] _(2B)_ filter kernel
  - [ ] need arbitrary size
  - [ ] apply button for do whole convolution
  - [ ] normalize
- [ ] _(2B)_ find edges
- [ ] _(2B)_ multiresolution automatic painting technique http://mrl.nyu.edu/projects/npr/painterly/
- [ ] _(2B)_ stretch and pull image http://mostfungames.com/warp-george-bush.htm
- [ ] _(2B)_ curved brush that follows image gradient http://mrl.nyu.edu/projects/npr/painterly/
- [ ] <u>(4B)</u> thumbnailhttp://www.cs.princeton.edu/~af/cool/webgothic.html
- [ ] <u>(4B)</u> video
- [ ] <u>(8B)</u> Bayesian matting