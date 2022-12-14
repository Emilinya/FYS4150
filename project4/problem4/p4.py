import numpy as np
import matplotlib.pyplot as plt

T = 1

def avg_eps(T):
    return np.sinh(-4/T)/(np.cosh(4/T) + 3)

def avg_absm(T):
    return 0.5*(np.exp(4/T) + 2)/(np.cosh(4/T) + 3)
def avg_absm2(T):
    return 0.5*(np.exp(4/T) + 1)/(np.cosh(4/T) + 3)

def ana_Cv(T):
    return (4 / T**2) * (1 + 3*np.cosh(4/T)) / ((np.cosh(4/T) + 3)**2)

def ana_X(T):
    return (1 / T) * (3*np.exp(4/T) + np.exp(-4/T) + 3) / ((np.cosh(4/T) + 3)**2)

def comp(eps, Cv, absm, X, T):
    def print_comp(name, num, ana):
        return print(f"{name:^5} | {f'{num:.4f}':>7} | {f'{ana:.4f}':>7}  | {f'{abs((num - ana)/ana)*100:.2f}':>5} %")

    print(" name | numeric | analytic | rel diff")
    print_comp("<ε>", eps, avg_eps(T))
    print_comp("Cv", Cv, ana_Cv(T))
    print_comp("<|m|>", absm, avg_absm(T))
    print_comp("χ", X, ana_X(T))

eps, absm = np.loadtxt("problem4/data.dat").T

# calc quantities

idx = np.arange(1, len(eps)+1)

cum_eps = np.cumsum(eps)/idx
cum_Cv = 4 * ((np.cumsum(eps**2)/idx) - cum_eps**2) / (T**2)

cum_absm = np.cumsum(absm)/idx
cum_Xi = 4 * ((np.cumsum(absm**2)/idx) - cum_absm**2) / T

comp(cum_eps[-1], cum_Cv[-1], cum_absm[-1], cum_Xi[-1], T)

# value plot

plt.rcParams['font.size'] = '14'
colors = plt.rcParams['axes.prop_cycle'].by_key()['color']
limits = (0.0435, 1 - 0.0435)
plt.figure(tight_layout=True)

plt.plot(idx, cum_eps, color=colors[0], label="$\\left<\\epsilon\\right>$")
plt.axhline(avg_eps(1), *limits, color=colors[0], linestyle="--")

plt.plot(idx, cum_absm, color=colors[1], label="$\\left<|m|\\right>$")
plt.axhline(avg_absm(1), *limits, color=colors[1], linestyle="--")

plt.plot(idx, cum_Cv, color=colors[2], label="$C_V$")
plt.axhline(ana_Cv(1), *limits, color=colors[2], linestyle="--")

plt.plot(idx, cum_Xi, color=colors[3], label="$\\chi$")
plt.axhline(ana_X(1), *limits, color=colors[3], linestyle="--")

plt.legend()
plt.xlabel("cycles []")
plt.ylabel("quantities []")
plt.savefig("imgs/p4_val.svg")
plt.clf()

# rel diff plot

plt.axhline(1, *limits, color="k", linestyle="dashed", label="1 %")
plt.axhline(0, color="k", linewidth=1)
plt.plot(idx, abs((cum_eps - avg_eps(1)) / avg_eps(1)) * 100, color=colors[0], label="$\\left<\\epsilon\\right>$")
plt.plot(idx, abs((cum_absm - avg_absm(1)) / avg_absm(1)) * 100, color=colors[1], label="$\\left<|m|\\right>$")
plt.plot(idx, abs((cum_Cv - ana_Cv(1)) / ana_Cv(1)) * 100, color=colors[2], label="$C_V$")
plt.plot(idx, abs((cum_Xi - ana_X(1)) / ana_X(1)) * 100, color=colors[3], label="$\\chi$")
plt.ylim(-2, 100)

plt.legend()
plt.xlabel("cycles []")
plt.ylabel("relative errors [%]")
plt.savefig("imgs/p4_err.svg")
plt.clf()
