default: shard

shard: shard.c
	gcc -o shard shard.c

.PHONY: clean
clean:
	rm -f shard
