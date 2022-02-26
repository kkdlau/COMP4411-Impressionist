# COMP4411 - Impressionist

## Bonus

W per new brush						yes
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
- Program uses C++17

# User Guide:  
- RGB scale: 
 - Check the Color Blending option in Brush Dialog
 - Choose File > Color Blending (ALT+K) to make the color chooser pop out  

- Alpha brush: 
 - Choose File > Load Alpha Image (ALT+X) to load any image. The alpha value is derived from its grayscale values.  
 - Choose Alpha-mapped Brush from the Brush Menu in Brush Dialog (no drawing made if no alpha image loaded)

- User-specified kernel:   
 - Choose File > Custom Filter (ALT+F) to input a filter  
 - Click 'Apply' to save any changes to the input  
 - Choose Custom Filter Brush from the Brush Menu in Brush Dialog (no drawing made if no filter is given)

- Automatic edge clipping:
 - Check the Edge Clipping option in Brush Dialog  

- To load other images for 'other image gradient' and 'other image edge clipping'
 - Choose File > Another Image (ALT+SHIFT+A) to load another image  

- Mosaic:  
 - This features works with xxx image
 - Choose File > Mosaic (ALT+M) and load the dataset xxx 
 - After using mosaic, click the window to refresh the application for other features

- Video:   
 - The video pauses when there are no mouse movements

- If there are any bugs, please try restarting the program... Thanks!