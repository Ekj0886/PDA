rm check
touch check
chmod +x check
make
./Lab2 1 ami33/ami33.block ami33/ami33.nets output/ami33
./draw
# ./Lab2 1 ami49/ami49.block ami49/ami49.nets output/ami49
# ./draw
# ./Lab2 1 vda317b/vda317b.block vda317b/vda317b.nets output/vda317b
# ./draw
./check



