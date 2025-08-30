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

    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_16.png" 16 16
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_24.png" 24 24
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_32.png" 32 32
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_48.png" 48 48
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_64.png" 64 64
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_128.png" 128 128
    svg2png "SG-AppIcon.svg" "$d/PlanGuidePlanner_256.png" 256 256

    oxipng --opt max --strip all --alpha --interlace 0 "$d/PlanGuidePlanner_"*".png"

    rm StudyGuide.ico && icotool -o StudyGuide.ico -c \
        "$d/PlanGuidePlanner_256.png"  \
        "$d/PlanGuidePlanner_128.png"  \
        "$d/PlanGuidePlanner_64.png"   \
        "$d/PlanGuidePlanner_48.png"   \
        "$d/PlanGuidePlanner_32.png"   \
        "$d/PlanGuidePlanner_24.png"   \
        "$d/PlanGuidePlanner_16.png"
else
    echo "ERROR: Windows icons were NOT generated!" >&2
    echo "ERROR: requires inkscape, icotool and oxipng in PATH"
fi

if command -v "inkscape"; then
    # Android res
   
    svg2png "SG-AppIcon.svg" "Android/res/drawable-ldpi/planguideplanner.png" 36 36
    svg2png "SG-AppIcon.svg" "Android/res/drawable-mdpi/planguideplanner.png" 48 48
    svg2png "SG-AppIcon.svg" "Android/res/drawable-hdpi/planguideplanner.png" 72 72
    svg2png "SG-AppIcon.svg" "Android/res/drawable-xhdpi/planguideplanner.png" 96 96
    svg2png "SG-AppIcon.svg" "Android/res/drawable-xxhdpi/planguideplanner.png" 144 144
    svg2png "SG-AppIcon.svg" "Android/res/drawable-xxxhdpi/planguideplanner.png" 192 192

else
    echo "ERROR: Android icons were NOT generated!" >&2
    echo "ERROR: requires inkscape in PATH"
fi


