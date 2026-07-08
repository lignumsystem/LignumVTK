# ImageMagick: preparing figures for academic journals
ImageMagick is a command-line utility for editing and manipulating digital images. This guide covers
essential commands for preparing journal-ready figures that fulfill editorial requirements. 
Brackets [...] indicate optional parameters.

>[!NOTE]
>ImageMagick 7 replaces the legacy `convert` command with a standalone `magick` command.
>Otherwise it uses a `magick <subcommand>` syntax instead of the direct commands found in ImageMagick 6.

## Basics
To get command-line help use *-help* following the tool:

    magick -help
    magick convert -help 

Use *-list* to query predefined options:

    magick -list format     #List supported file formats on your local system
    magick -list colorspace #List all supported colorspaces 
    
Query image. The *-verbose* flag outputs full image details, while the *-ping* option efficiently checks
the file without loading the image into memory:

    magick identify [-verbose] [-ping] Figure.tiff
    
## File conversion
Convert PNG to PDF:

    magick Figure.png  FigureOut.pdf 
    
Convert PNG to TIFF, grayscale image:

    magick Figure.png -grayscale Gray FigureOut.tiff       #Linear intensity average, no correction for human perception
    magick Figure.png -grayscale Rec709Luma FigureOut.tiff #Modern HDTV coefficients, matches human perception

Rotate PNG file 90 degress with lossless (LZW) compression to TIFF:

    magick Figure.png -rotate 90 -compress Lossless FigureOut.tiff

>[!NOTE]
>Command-line order matters: place the input file first, options second, and the output file last.
>ImageMagick evaluates arguments strictly from left to right, so option order affects the outcome.

## Image quality
### Technical requirements
Journals have strict formatting rules to ensure images remain crisp in both digital and print layouts. 
The following technical requirements are common:

+ **Resolution:** The usual image quality requirement is 300 DPI[^dpi].
+ **File Format:** Uncompressed TIFF is the golden standard for digital raster images, PDF for scalable vector graphics.
+ **Color space:** Editors typically require the sRGB or Adobe RGB standards for digital publication,
  whereas physical print editions utilize CMYK color space.
  
### Setting image properties
The general formula for image size is:

    WidthInPixels = Image size (inches) X DPI desired.

For instance, a 7-inch wide, two column image with 300 DPI requirement is 2100 pixels wide. 
For a 3.5-inch single column image, the width is 1050 pixels:

    magick Figure.png  -density 300 -resize 2100x -units PixelsPerInch FigureOut.tiff #Two column image
    magick Figure.png  -density 300 -resize 1050x -units PixelsPerInch FigureOut.tiff #Single column image
    
The *-resize* options set the image width explicitly while maintaining the aspect ratio. When an academic
journal requires 300 DPI, they are actually referring to 300 PPI[^ppi] in the digital image to ensure the image
has enough pixel density for final printing.

Journals usually check the image metadata. Verify the image confirms to the requested size, the 300 DPI resolution,
and the required color space:

    magick identify -format "%w x %h pixels, %x DPI\n" FigureOut.tiff #Size and resolution
    magick identify -format "%x x %y %U\n" FigureOut.tiff             #Units
    magick identify -format "%[colorspace]\n" FigureOut.tiff          #Color space

After completing these steps, the image is usually ready for submission. 

>[!IMPORTANT]
>Always inspect the converted output image for anomalies, paying close attention to text readability
>and line quality.

### Setting color space
Color spaces define the digital color environment for editing software and hardware. While a *color space* acts
as a theoretical map of a specific color range, an ICC[^icc] *color profile* is the *data file* that bridges this map
to the physical colors a device can actually display or print. 

In consumer devices, color management is typically automatic. For instance, macOS uses ColorSync as a neutral
coordinator, translating color profile data seamlessly between input devices, software applications,
and output hardware.

High-quality professional printing requires precise color management. Because final outputs depend on specific
ink and paper characteristics, designers use robust ICC profiles alongside standardized color spaces to ensure
consistency.

Some programming libraries, such as *matplotlib* and *ggplot2*, export figures without color metadata;
in such cases, the default sRGB color space is typically assumed. *However, this omission may violate
a journal's formatting requirements*. Although journals specify their required color profiles, they
rarely provide them. Authors can download ICC color profiles from the 
[ICC Profile Registry](https://registry.color.org/profile-registry/),
[ECI Downloads](https://eci.org/doku.php_id=en_downloads.html)[^eci] and
[Adobe](https://www.adobe.com/digitalimag/adobergb.html),

To set or convert a color profile, you *must* use ICC color profiles. This technique utilizes exact color lookup
tables rather than generic equations to match how human eyes, computer monitors, and printers interpret colors.

    magick FigureOut.png -profile sRGB.icc FigureOut_sRGB.tiff                              #Set color profile to sRGB
    magick FigureOut.png -profile sRGB.icc -profile USWebCoatedSWOP.icc FigureOut_cmyk.tiff #Color profile conversion to CMYK

In the conversion, the first *-profile* assigns the baseline color space, while the second triggers 
the actual pixel transformation; the sRGB.icc profile is applied if the image lacks an embedded profile.

To query the color profile in the image file,run the following command:

    magick identify -format "%[profile:icc]\n" FigureOut_sRGB.tiff   

### Grayscale figures
Verify the image metadata as with color images. The expected color space should be *Gray*. The following
command in ImageMagick produces grayscale image with 300 DPI:

    magick Figure.tiff -colorspace Gray -density 300 -units PixelsPerInch FigureBW.tiff

Control gray-shade saturation using *-grayscale*:

    magick Figure.tiff -grayscale Rec709Luma -colorspace Gray -density 300 -units PixelsPerInch FigureBW.tiff

The trailing *-colorspace* ensures the final file format compatibility.

Grayscale conversion can sometimes affect text or line quality. Increase the contrast and sharpness:

    magick Figure.tiff -colorspace Gray -density 300 -units PixelsPerInch -level 10%,90% -sharpen 0x1 FigureBW.tiff

[^dpi]: Dots per inch.
[^ppi]: Pixels per inch.
[^icc]: International Color Consortium.
[^eci]: European Color Initiative.
