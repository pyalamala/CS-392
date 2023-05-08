#Pranav Yalamala
#I pledge my honor that I have abided by the Steven's Honor System.

mkdir -p dir1
mkdir -p dir1/dir2
mkdir -p dir1/dir3

cd dir1
truncate -s 5K junk.txt
truncate -s 10K junk1.txt
cd dir2
truncate -s 50K junk21.txt
truncate -s 500K junk22.txt
truncate -s 5K junk23.txt
truncate -s 100K junk24.txt
chmod +w junk21.txt
chmod -w junk22.txt
chmod -w junk23.txt
chmod +w junk24.txt
cd ..
cd dir3
truncate -s 80K junk31.txt
truncate -s 1000K junk32.txt
truncate -s 5K junk33.txt
truncate -s 1000K .junk34.txt
chmod +w junk31.txt
chmod -w junk32.txt
chmod -w junk33.txt
chmod +w .junk34.txt
