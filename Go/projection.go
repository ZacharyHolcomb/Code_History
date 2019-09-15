/*
 * Author: Brian Massino, bmassino2017@my.fit.edu
 * Author: Zachary Holcomb, zholcomb2017@my.fit.edu
 * Course: CSE 4250, Fall 2019
 * Project: projection.go, projection project
 * Implementation: golang go1.12.9
 */

package main

import (
	"fmt"
	"image"
	"image/color"
	"image/png"
	"math"
	"os"
	"strconv"
)

//CheckError error checking function
func CheckError(err error) {
	if err != nil {
		fmt.Print(err, "\n")
	}
}

func main() {
	Arguments := os.Args //pulling in command line information
	InputImageReader, err := os.Open(Arguments[1])
	CheckError(err)
	InputImage, err := png.Decode(InputImageReader)
	CheckError(err)
	OutputImageReader, err := os.Create(Arguments[2])
	CheckError(err)
	StandardLatitude := 0.0

	if len(Arguments) > 3 { // use lambert projection
		StandardLatitude, err = strconv.ParseFloat(Arguments[4], 64)
		StandardLatitude = StandardLatitude * math.Pi / 180        //converting standard latitude
		Ratio := math.Pi * math.Pow(math.Cos(StandardLatitude), 2) //ratio for any given standard latitude (standard parallels). Multiply this by height of image

		ModifiedImage := image.NewRGBA64(image.Rectangle{image.Point{0, 0}, image.Point{int(Ratio) * InputImage.Bounds().Max.Y, InputImage.Bounds().Max.Y}})

		newx, newy := 0.0, 0.0

		xratio := math.Pi * math.Cos(StandardLatitude) //ratio to get pixel x and y coordinatse into unitless x and y, so that we can plug into the equation
		yratio := math.Sin(math.Pi/2) * (1 / math.Cos(StandardLatitude))

		for x := ModifiedImage.Bounds().Min.X; x < ModifiedImage.Bounds().Max.X; x++ { //looping through the new picture, pixel by pixel
			for y := ModifiedImage.Bounds().Min.Y; y < ModifiedImage.Bounds().Max.Y; y++ {

				newx = ((float64(x) / (float64(ModifiedImage.Bounds().Max.X)) * 2) - 1) * xratio //converting to unitless x and y
				newy = ((float64(y) / (float64(ModifiedImage.Bounds().Max.Y)) * 2) - 1) * yratio

				longitude := newx * (1 / math.Cos(StandardLatitude)) // plugging x and y into the inverse equation
				latitude := math.Asin(newy * math.Cos(StandardLatitude))

				finalx := ((longitude / float64(math.Pi)) + 1) * float64(InputImage.Bounds().Max.X) //converting back to pixel coordinates from the equirectangular projection
				finaly := ((latitude / float64(math.Pi/2)) + 1) * float64(InputImage.Bounds().Max.Y)

				ModifiedImage.Set(x, y, InputImage.At(int(finalx/2), int(finaly/2))) // setting the pixels, in this case scaling back to fit modified resolution

			}
		}

		err = png.Encode(OutputImageReader, ModifiedImage) //save the image
		CheckError(err)

	} else { //do mollweide projection

		ModifiedImage := image.NewRGBA64(image.Rectangle{image.Point{0, 0}, image.Point{InputImage.Bounds().Max.X, InputImage.Bounds().Max.Y}})

		OutputMaxY := ModifiedImage.Bounds().Max.Y
		OutputMaxX := ModifiedImage.Bounds().Max.X

		for OutputX := ModifiedImage.Bounds().Min.X; OutputX < InputImage.Bounds().Max.X; OutputX++ {
			for OutputY := ModifiedImage.Bounds().Min.Y; OutputY < InputImage.Bounds().Max.Y; OutputY++ {
				ScaledX := ((float64(OutputX) / float64(OutputMaxX) * 2) - 1) * float64(math.Pi)   //again, unitless ratio conversion for x and y, but because we know the image
				ScaledY := ((float64(OutputY) / float64(OutputMaxY) * 2) - 1) * float64(math.Pi/2) //is non chaning in width and height, the ratio would be pi and pi/2

				Theta := math.Asin(float64(ScaledY) / (math.Sqrt(2))) //calculating each portion from the inverse mollweide equation

				Long := ((math.Pi * float64(ScaledX)) / (2 * math.Sqrt(2) * math.Cos(Theta)))
				Lat := math.Asin((2*Theta + math.Sin(2*Theta)) / math.Pi)

				Finalx := (((Long / (math.Pi)) + 1) / 2) * float64(OutputMaxX) //translating back to pixel
				Finaly := (((Lat / (math.Pi / 2)) + 1) / 2) * float64(OutputMaxY)

				ModifiedImage.Set(OutputX, OutputY, InputImage.At(int(Finalx), int(Finaly)))

			}
		}
		for x := ModifiedImage.Bounds().Min.X; x < InputImage.Bounds().Max.X; x++ { //another double loop to create the whitespace around the mollweide projection
			for y := ModifiedImage.Bounds().Min.Y; y < InputImage.Bounds().Max.Y; y++ { //definitely could be incorporated into the above double for, but we ran out of time
				r, g, b, a := ModifiedImage.At(x, y).RGBA() //getting rgba values from each pixel
				_ = r + g + b
				if a == 0 { //if it is transparent, set it to white
					ModifiedImage.Set(x, y, color.White)
				}
			}
		}
		err = png.Encode(OutputImageReader, ModifiedImage) //save the image
		CheckError(err)

	}

	InputImageReader.Close() //close the files!
	OutputImageReader.Close()

}
