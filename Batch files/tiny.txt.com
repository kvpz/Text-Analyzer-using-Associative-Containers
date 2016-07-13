r ../proj4/data0.in
r ../proj4/data1.in
r data2.in
r data3.in
r data4.in
r test.text
r data6.in
s
w data.report
q


#r ../proj4/tiny.txt
#s
#w tiny.txt.out
#x
