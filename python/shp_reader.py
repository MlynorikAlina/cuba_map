import shapefile

from PIL import Image, ImageDraw, ImageFont

# create an image
w = 900
h = 900
lat_max = 30
lon_min = -90
w_deg = 30
h_deg =  30
w_k = w/w_deg
h_k = h/h_deg
out = Image.new("RGB", (w, h), (179, 248, 252))

# get a drawing context
d = ImageDraw.Draw(out)

m = 20037508.34

shape = shapefile.Reader("land-polygons/land_polygons.shp")
#first feature of the shapefile
for feature in shape.shapeRecords():
	first = feature.shape.__geo_interface__
	arr = first.get("coordinates")[0]

	for i in range(0, len(arr)):
		t = arr[i]
		x = round(w_k*(t[0]-lon_min))
		y = round(h_k*(lat_max - t[1]))
		arr[i] = (x,y)

	d.polygon(arr,fill=(159, 159, 159))


shape.close()

out.save("cuba.png", "PNG")
