# out_of_core_k-means
k-means algo using as much as IO, not memory

You can use gen_data generate tons of  random data(float), and use k-means algo based on IO from disk (SSD recommended) only use small limited memory.


# generate data
```./gen_data```


# key point

* This program use mmap() to make full use of System memory page to speed up reading process.
* break huge data file into pieces, multiple threads handle each piece.
* unflod some FOR loop to make use of cores

# Warning
Not for your homework, just a toy code, not perfect.