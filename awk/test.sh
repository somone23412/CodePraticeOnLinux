cat test.txt | \
awk ' \
BEGIN {
}
{ \
    print "lines"NR":", "$1="$1, "$2="$2, "$2>400?"($2>400); \
    count[$1]++; \
    if($2>400) \
        above400[$1]++ \
} \
END { \
    print "total lines = "NR;
    for(key in count) { \
        print key, count[key], above400[key]/count[key] \
    } \
}' \
> result.txt

