# ImageMagick: preparing figures for academic journals
[ImageMagick](https://imagemagick.org/) is a command-line utility for editing and manipulating digital images.
This guide covers essential commands for preparing journal-ready figures that meet editorial requirements. 
Brackets [...] indicate optional parameters.

> [!NOTE]
> ImageMagick 7 replaces version 6 standalone binaries with a unified `magick <subcommand>` syntax
> and processes command-line options strictly from left to right.

Understanding ImageMagick 7 command execution flow:

	magick <subcommand> [global_options] [input_settings] input_image.jpg [image_operators] output_image.jpg

+ Subcommand: Entry point to all operations, like *convert*, *identify* and *mogrify*.
+ Global options: Applied first to configure the overall execution environment.
+ Input settings: Set image attributes before reading the input file for `magick` processing.
+ Input image: Read the input file prior to image operators. 
+ Image operators: Applied sequentially to the image currently stored in the memory.
+ Output image: Writes the final state of the image to disk.

Because `magick` defaults to *convert*, you can omit this subcommand along with rarely needed
Global options and Input settings.

## Basics
To get command-line help use *-help* following the tool:

    magick -help         #Help for magick
    magick convert -help #Help for convert

Use *-list* to query predefined options:

    magick -list format     #List supported file formats on your local system
    magick -list colorspace #List all supported colorspaces 
    
Query image. The *-verbose* flag outputs full image details, while the *-ping* option efficiently checks
the file without loading the image into memory:

    magick identify [-verbose] [-ping] figure.tiff
    
## File conversion
Convert PNG to PDF.

    magick figure.png  figure_out.pdf 
    
Convert PNG to TIFF, grayscale image:

    magick figure.png -grayscale Gray figure_out.tiff       #Linear intensity average, no correction for human perception
    magick figure.png -grayscale Rec709Luma figure_out.tiff #Modern HDTV coefficients, matches human perception

Rotate PNG file 90 degress with lossless (LZW) compression to TIFF:

    magick figure.png -rotate 90 -compress Lossless figure_out.tiff


## Color digital images 
### Technical requirements
Journals have strict formatting rules to ensure images remain crisp in both digital and print layouts. 
The following technical requirements are common:

+ **Column width:** 3.5-inch single column and 7-inch double column are common. 
+ **Resolution:** The usual image quality requirement is 300 DPI[^dpi].
+ **File Format:** Uncompressed TIFF is the golden standard for digital raster images, PDF for scalable vector graphics.
+ **Color space:** Editors typically require the *sRGB* or *Adobe RGB* for digital publication,
  whereas physical print editions utilize *CMYK* color space.
  
### Setting image properties
The general formula for image size is:

    WidthInPixels = Image size (inches) X DPI desired.

For instance, a 7-inch wide, two column image with 300 DPI requirement is 2100 pixels wide. 
For a 3.5-inch single column image, the width is 1050 pixels:

    magick figure.png  -density 300 -resize 2100x -units PixelsPerInch figure_out.png #Two column image
    magick figure.png  -density 300 -resize 1050x -units PixelsPerInch figure_out.png #Single column image
    
The *-resize* options set the image width explicitly while maintaining the aspect ratio. When an academic
journal requires 300 DPI, the editors actually mean 300 PPI[^ppi] in the digital file to ensure adequate pixel
density for printing.

Journals routinely use automated software to scan and verify image metadata during editorial triage.
Verify the image confirms to the requested size, resolution, and color space:

    magick identify -format "%w x %h pixels, %x DPI\n" figure_out.png #Size and resolution
    magick identify -format "%x x %y %U\n" figure_out.png             #Units: PixelsPerInch
    magick identify -format "%[colorspace]\n" figure_out.png          #Color space

After completing these verification steps, the image is ready for submission. 

> [!IMPORTANT]
> Always inspect the converted output image for anomalies, paying close attention to text readability
> and line quality.

### Setting color space
> [!NOTE]
> Manual color metadata settings are rarely needed. However, some software programs
> omit this information, which can conflict with journal requirements.

Color spaces define the digital color environment for editing software and hardware. While a *color space* acts
as a theoretical map of a specific color range, an ICC[^icc] *color profile* is the *data file* that bridges this map
to the physical colors a device can actually display or print. 

In consumer devices, color management is typically automatic. For instance, macOS uses ColorSync as a neutral
coordinator, translating color profile data seamlessly between input devices, software applications,
and output hardware.

High-quality professional printing requires precise color management. Because final outputs depend on specific
ink and paper characteristics, journal designers use robust ICC profiles alongside standardized color spaces to ensure
consistency.

Some programming libraries, such as *matplotlib* and *ggplot2*, export figures without color metadata;
in such cases, the default sRGB color space is typically assumed. *However, this omission may violate
a journal's formatting requirements*. Although journals specify their required color profiles, they
rarely provide them. If the required ICC color profiles are not installed in your operating system,
you can download them from the profile registries:

+ [ICC Profile Registry](https://registry.color.org/profile-registry/)
+ [ECI[^eci] Downloads](https://eci.org/doku.php_id=en_downloads.html)
+ [Adobe](https://www.adobe.com/digitalimag/adobergb.html)

To set or convert a color profile, you *must* use ICC color profiles. This technique utilizes exact color lookup
tables rather than generic equations to match how human eyes, computer monitors, and printers interpret colors.

    magick figure.png -profile sRGB.icc figure_out_srgb.tiff                              #Set color profile to sRGB
    magick figure.png -profile sRGB.icc -profile USWebCoatedSWOP.icc figure_out_cmyk.tiff #Color profile conversion to CMYK

In the conversion, the first *-profile* assigns the baseline color space, while the second triggers 
the actual pixel transformation; the sRGB.icc profile is applied if the image lacks an embedded profile.

To query the color profile in the image file, run the following command:

    magick identify -format "%[profile:icc]\n" figure_out_srgb.tiff

## Grayscale digital images
The following command produces two-column, grayscale image with 300 DPI:

    magick figure.tiff -resize 2100x -colorspace Gray -density 300 -units PixelsPerInch figure_out_bw.tiff

Choose the gray-shade saturation algorithm with *-grayscale*:

    magick figure.tiff -resize 2100x -grayscale Rec709Luma -colorspace Gray -density 300 -units PixelsPerInch figure_out_bw.tiff

Grayscale conversion can sometimes affect text or line quality. Increase the contrast and sharpness
with *-level* and *-sharpen*:

    magick figure.tiff -resize 2100x -colorspace Gray -density 300 -units PixelsPerInch -level 10%,90% -sharpen 0x1 figure_out_bw.tiff
	magick figure.tiff -resize 2100x -colorspace Gray -density 300 -units PixelsPerInch -auto-level -sharpen 0x1 figure_out_bw.tiff
	
The *-auto-level* operator automatically calculates minimum and maximum pixel values to stretch the image contrast.

Verify the image metadata as you would with color images; the expected color space should be *Gray*. Once verified,
the image is ready for submission. 

[^dpi]: Dots per inch: Physical print quality metric that tells how closely a printer packs ink dots on paper.

[^ppi]: Pixels per inch: A digital image metric that specifies the number of pixels in one physical inch of screen.

[^icc]: International Color Consortium.

[^eci]: European Color Initiative.
