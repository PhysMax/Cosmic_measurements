for ((theta = 0; theta <= 90; theta += 1))
do
./simpleHE.exe run.mac 1 $theta
done
rm output.root
hadd output.root simpleHE*.root
