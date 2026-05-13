import marimo

__generated_with = "0.23.5"
app = marimo.App(width="medium")


@app.cell
def _():
    import matplotlib.pyplot as plt
    import marimo as mo
    import numpy as np
    import pandas as pd

    df = pd.read_csv("data/output.csv")
    counts = df["count"]

    counts.hist(bins=100)

    mean = counts.mean()
    std = counts.std()
    count = len(counts)

    stats_text = (
        f"Mean: {mean:.2f}\n"
        f"Std. Dev.: {std:.2f}\n"
        f"Particle count: {count}"
    )

    plt.title("Distribution of particle count per event")
    plt.xlabel("Particle count")
    plt.ylabel("Event frequency")

    plt.text(0.95, 0.95, stats_text,
             transform=plt.gca().transAxes,
             verticalalignment="top",
             horizontalalignment='right',
             bbox=dict(boxstyle="round", facecolor="white"),
             family='monospace'
            )

    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
