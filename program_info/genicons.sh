#!/bin/bash

svg2png() {
    input_file="$1"
    output_file="$2"
    width="$3"
    height="$4"

    inkscape -w "$width" -h "$height" -o "$output_file" "$input_file"
}

if command -v "inkscape" && command -v "icotool" && command -v "oxipng"; then
    # Windows ICO
    d=$(mktemp -d)

    svg2png "SG-AppIcon.svg" "$d/StudyGuide_16.png" 16 16
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_24.png" 24 24
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_32.png" 32 32
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_48.png" 48 48
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_64.png" 64 64
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_128.png" 128 128
    svg2png "SG-AppIcon.svg" "$d/StudyGuide_256.png" 256 256

    oxipng --opt max --strip all --alpha --interlace 0 "$d/StudyGuide_"*".png"

    rm StudyGuide.ico && icotool -o StudyGuide.ico -c \
        "$d/StudyGuide_256.png"  \
        "$d/StudyGuide_128.png"  \
        "$d/StudyGuide_64.png"   \
        "$d/StudyGuide_48.png"   \
        "$d/StudyGuide_32.png"   \
        "$d/StudyGuide_24.png"   \
        "$d/StudyGuide_16.png"
else
    echo "ERROR: Windows icons were NOT generated!" >&2
    echo "ERROR: requires inkscape, icotool and oxipng in PATH"
fi
