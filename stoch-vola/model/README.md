# Stochastic Volatility model in C++ (with python bindings)

Implements a Stochastic Volatility model with particle filtering and log-likelihood calculation for the following specification:

$$x_t\sim\mathcal{N}(\mu+\phi x_{t-1},\sigma^2)$$
$$y_t\sim\mathcal{N}(0,\exp(x_t))$$

Example notebook, using Python, [here](https://sarem-seitz.com/notebooks/StochasticVolatility.html).
