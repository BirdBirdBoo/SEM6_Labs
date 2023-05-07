import math

import sklearn
from sklearn.datasets import make_classification
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np


def sigma(x):
    return 1 / (1 + math.e ** (-x))


def lin(x: np.ndarray, theta: np.ndarray):
    return np.dot(np.append(x, 1), theta)


def sigma_linear(x: np.ndarray, theta: np.ndarray):
    return sigma(lin(x, theta))


def gradient(theta: np.ndarray, pts: np.ndarray, classes: np.ndarray, gamma=0.5) -> np.ndarray:
    n = pts.shape[0]

    def sigma_mul_one_sigma(pt: np.ndarray, _theta: np.ndarray):
        s = sigma_linear(pt, _theta)
        return s * (1 - s)

    def safe_x(pt, i):
        return pt[i] if i < len(pt) else 1

    def y_prop(pt, c):
        y = sigma_linear(pt, theta)

        return c - y

        # return c / y if c != 0 else (1 - c) / (1 - y)

    return - gamma / n * np.array(
        [sum([sigma_mul_one_sigma(pt, theta) * safe_x(pt, i) * y_prop(pt, c)
              for pt, c in zip(pts, classes)])
         for i in range(pts.shape[1] + 1)])


def generate_data(n=2000):
    return make_classification(n, n_features=2, n_redundant=0, class_sep=1, random_state=10, flip_y=0.0,
                               n_clusters_per_class=1)


def show_pts(pts, classes, theta=None):
    df = pd.DataFrame({'x0': pts[:, 0], 'x1': pts[:, 1], 'y': classes})

    plt.figure(figsize=(10, 5))
    sns.scatterplot(x='x0', y='x1', hue='y', data=df)

    if theta is not None:
        def boundary(x):
            return -(theta[0] * x + theta[2]) / theta[1]

        boundary_data = np.array([[x, boundary(x)] for x in [min(pts[:, 0]), max(pts[:, 0])]])

        boundary_df = pd.DataFrame({'x0': boundary_data[:, 0], 'x1': boundary_data[:, 1]})

        sns.lineplot(data=boundary_df, x='x0', y='x1', color='red')

    plt.xlim(-4, 4)
    plt.ylim(-6, 6)

    plt.show()


def main():
    pts, classes = generate_data()

    show_pts(pts, classes)

    theta = np.ones(3)
    show_pts(pts, classes)

    while np.linalg.norm(d_theta := gradient(theta, pts, classes, 16)) > 0.1:
        # print(d_theta)
        show_pts(pts, classes, theta)
        theta += d_theta

    print(f'Final fit: {theta}')

    show_pts(pts, classes, theta)


if __name__ == '__main__':
    main()
