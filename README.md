# COMP4411 - Impressionist

## Bonus

W per new brush (fan, arc)						yes  
W border clipping					yes  
W (red) marker						yes  
W swap							yes  
B RGB scale						yes  
B blur brush					yes  
B 1 level undo						yes  
B dissolve						yes  
BW fade in/out input on output canvas			yes  
BW mural (blending)					yes  
BW alpha brush						yes  
BW automatic paint whole image				yes  
BW gradient from user-specified image			yes  
BW edge clipping					yes  
2B user-specified kernel				yes  
2B edge detection + user-specified edge clipping	yes  
2B multiresolution					yes  
2B warp brush						yes  
2B curved brush following image gradient		yes  
4B mosaic						yes  
4B impressionist video with temporal coherency		yes  
~~8B Bayesian matting					no~~  

# Notes:
- Program uses C++17. And we have configured the solution file to C++17.
  - However, if the graders use the sample solution file, there will be compiler errors
  - So please use the solution attached with the submission.
- We use some macro tricks that enable / disable debugging messages
  - If you pass `-DPROJ_DEBUG` to the compiler, the debugging message will be enabled
  - To make the program be more efficient, don't pass the flag

# User Guide:  
1. RGB scale: 
 - Check the Color Blending option in Brush Dialog
 - Choose File > Color Blending (ALT+K) to make the color chooser pop out  

2. Alpha brush: 
 - Choose File > Load Alpha Image (ALT+X) to load any image (the alpha value is derived from its grayscale values)
 - Choose Alpha-mapped Brush from the Brush Menu in Brush Dialog (no drawing is made if no alpha image is loaded)

3. User-specified kernel:   
 - Choose File > Custom Filter (ALT+F) to input a filter  
 - Click 'Apply' to save any changes to the input  
 - Choose Custom Filter Brush from the Brush Menu in Brush Dialog (no drawing is made if no filter is given)

4. Automatic edge clipping:
 - Check the Edge Clipping option in Brush Dialog  

5. To load other images for 'other image gradient' and 'other image edge clipping'
 - Choose File > Another Image (ALT+SHIFT+A) to load another image  

6. Mosaic:  
 - This features creates tiles with 5X5 images
 - Choose File > Mosaic (ALT+M) and load the dataset folder `bonus/dataset` 
 - Please be patient as the processing time is long
 - After using mosaic, click the window to refresh the application for other features
 - I can tell that red and green doesn't match well
   - I tried SSIM, Average Color Distance, L1 Distance and L2 Distance
   - Still it doesn't work as what I expected, cryyyyyyyyy
 - Since It takes so long to run, we add a showcase video called `showcase_mosaic.m4v` to demo this feature

7. Video:   
 - The video pauses when there are no mouse movements
 - User may tune brush settings while video is running
 - A sample video located at `bonus/videos/cinepak.avi` is prepared for the graders

8. Rubber / Sketch and pull:
 - Actually the brush doesn't work very well
 - I decided to keep it as I put lots of effort.
 - Can you give me partial credit for this, please QAQ?

9. Multi-Resolution Painting:
 - The effect is best observed with Point brush, or Curved brush with width set to 3

10.  If there are any bugs, please try restarting the program... Thanks!
