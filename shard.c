#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>

#define USAGE "shard ( -s <max file size(bytes)> | -u ) <file>\n"

void usage()
{
    fprintf(stderr, USAGE);
}

void error(const char* msg)
{
    fprintf(stderr, "ERR: %s\n", msg);
}

void shard(const char* file, size_t max_shard_size)
{
    int n_files;                /* number of shards to create */
    struct stat attributes;     /* stat buffer */
    char* copy_buf;             /* shard data buffer */
    char shard_name_buf[255];   /* shard names */
    char tmp[255];              /* temp buffer */
    FILE* src_file;             /* file to read from */
    int iter;                   /* iteration variable */
    int nread;                  /* bytes read from source file */
    int nwrite;                 /* bytes written to shard file */
    FILE* current_shard;        /* shard being written to */

    /* determine the number of shards neccessary */
    if( stat(file, &attributes) != 0 )
    {
        error("stat failure.");
        exit(1);
    }

    n_files = (attributes.st_size / max_shard_size) + 1;

	/* if max_shard_size exactly divides src file size */
	/* subtract 1 to account for overcounted shards */
	if( attributes.st_size % max_shard_size == 0 )
	{
		--n_files;
	}

    /* create buffer to hold temporary shard data */
    copy_buf = (char*) malloc( max_shard_size );

    if( copy_buf == NULL )
    {
        error("malloc failure.");
        exit(1);
    }

    if( (src_file = fopen(file, "r")) == NULL )
    {
        error("could not open file");
        exit(1);
    }

    /* create shard files */
    for(iter = 0; iter < n_files; ++iter)
    {
        /* create shard name */
        shard_name_buf[0] = '\0';
        strcat(shard_name_buf, file);
        strcat(shard_name_buf, "_");
        sprintf(tmp, "%d", iter);
        strcat(shard_name_buf, tmp);
        
        /* read data from source file */
        nread = fread(copy_buf, 1, max_shard_size, src_file);
		
		/* break on EOF */
		if( nread == 0 )
		{
			break;
		}

        /* open shard file */
        if( (current_shard = fopen(shard_name_buf, "w")) == NULL )
        {
            error("could not open shard file");
            free(copy_buf);
            fclose(src_file);
            exit(1);
        }
        
        /* write data to shard file */
        nwrite = fwrite(copy_buf, 1, nread, current_shard);

        /* close shard file */
        fclose(current_shard);
    }

    fclose(src_file);
}

void unshard(const char* file)
{

}

int main(int argc, char* argv[])
{
    if( argc != 3 && argc != 4)
    {
        usage();
        return 1;
    }

    if( strcmp(argv[1], "-s") == 0 && argc == 4 )
    {
        shard(argv[3], atoi(argv[2]));
    }
    else if( strcmp(argv[1], "-u") == 0 && argc == 3 )
    {
        unshard(argv[2]);
    }
    else
    {
        usage();
        return 1;
    }

    return 0;
}
