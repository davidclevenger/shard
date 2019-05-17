default: shard

shard: shard.c
	gcc -g -o shard shard.c

.PHONY: clean
clean:
	rm -f shard
