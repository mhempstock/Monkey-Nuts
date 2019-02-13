# First attempt of solving the monkey problem

count = 0
nuts = 311
for a in range(1, nuts + 1, 2):
    for b in range(1, nuts + 1, 2):
        for c in range(1, nuts + 1, 2):
            z = a + b + c
            if z == nuts and a != c and a != b and b != c:
                count += 1
print "total combinations %s" % count
