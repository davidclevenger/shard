# make a file
echo "32849 random file data" > file.txt

# shard the file
./shard -s 2 file.txt

# recreate the file
cat *_* > connected.txt

# compare
diff file.txt connected.txt

# cleanup
rm -f file.txt connected.txt

