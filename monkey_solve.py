import numpy as np
from sympy import S, symbols
import sympy
from matplotlib import pyplot as plt

tnuts = 1001

x = []
y = []
for nuts in range(1, tnuts + 1, 2):
    count = 0
    for a in range(1, nuts + 1, 2):
        loop2 = (nuts - a) + 1
        for b in range(1, loop2 + 1, 2):
            c = nuts - a - b
            if c > 0 and a not in (b, c) and b not in (a, c):
                count += 1
    x.append(nuts)
    y.append(count)


p = np.polyfit(x, y, 2)
f = np.poly1d(p)
x_new = np.linspace(x[0], x[-1], 50)
y_new = f(x_new)
plt.plot(x, y)
x = symbols("x")
poly = sum(S("{:6.3f}".format(v)) * x**i for i, v in enumerate(p[::-1]))
eq_latex = sympy.printing.latex(poly)
plt.plot(x_new, y_new, label="${}$".format(eq_latex))
plt.legend(fontsize="small")
print int(np.polyval(p, 1000001))
plt.show()
