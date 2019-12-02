[[/GLM/General/Random values]] -- Random value distributions

# Synopsis
GLM:
~~~
degenerate(<value>)
uniform(<min>,<max>)
normal(<mean>,<stdev>)
bernoulli(<probability>)
sampled(<N>,<sample_1>,<sample_2>,...,<sample_N>)
pareto(<base>,<gamma>)
lognormal(<geometric-mu>,<geometric-sigma>)
exponential(<lambda>)
beta(<alpha>,<beta>)
gamma(<alpha>,<beta>)
weibull(<l>,<k>)
rayleigh(<s>)
triangle(<min>,<max>)
triangle_asy(<min>,<max>,<center>)
~~~

# Description

Random variables are generated using one of the following distributions.

## `degenerate`
~~~
degenerate(a)
~~~

The PDF is $\phi(x;a) = \left\\{ 1 & : x = a ; 0 & : x \ne a \right.$

## `uniform`
~~~
uniform(a,b)
~~~

The PDF is $\phi(x;a,b) = \left\\{ \frac{1}{b-a} &: a \le x < b ; 0 &: x < a ; x \ge b \right.$

## `normal`
~~~
normal(mu,sigma)
~~~

The PDF is $\phi(x;\mu,\sigma) = \frac{1}{\sqrt{2\pi}\sigma} e^{\frac{(x-\mu)^2}{2\sigma^2}}$.

The value is generated using the Box-Muller method.

## `bernoulli`
~~~
bernoulli(p)
~~~

The PDF is $\phi(x;p) = \left\\{ 1 & : x \ge p ; 0 & : x < p \right.$.

## `sampled`
~~~
sampled(N,a_1,a_2,...,a_N)
~~~

The PDF is $\phi(x;a_1,a_2,\cdots,a_N) = \left\\{ 1/N & : x = a_1 ; 1/N &:x = a_2 ; \cdots ; 1/N & : x = a_N \right. $.

## `pareto`
~~~
pareto(m,k)
~~~

The PDF is $\phi(x;k) = \left\\{ k \frac{m^k}{x^{k+1}}&:m \le x ; 0 : x < m \right.$.

## `lognormal`
~~~
lognormal(mu,sigma)
~~~

The PDF is $\phi(x) = \left\\{ \frac{1}{\sqrt{2\pi}x\sigma} e^{\frac{(\ln x-\mu)^2}{2\sigma^2}}$$ &: x > 0 ; 0 &: x \le 0 \right.$.

## `exponential`
~~~
exponential(lambda)
~~~



## `beta`
~~~
beta(<alpha>,<beta>)
~~~

## `gamma`
~~~
gamma(<alpha>,<beta>)
~~~

## `weibull`
~~~
weibull(<l>,<k>)
~~~

## `rayleigh`
~~~
rayleigh(<s>)
~~~

## `triangle`
~~~
triangle(<min>,<max>)
~~~

## `triangle_asy`
~~~
triangle_asy(<min>,<max>,<center>)
~~~

# See also
* [[/GLM/Property/Randomvar]]
