import marimo

__generated_with = "0.23.5"
app = marimo.App(width="medium")


@app.cell
def _():
    import uproot
    import matplotlib.pyplot as plt

    tree = uproot.open("data/detector_simulation.root:Steps")
    return plt, tree


@app.cell
def _(tree):
    data = tree.arrays(library="pd")
    return (data,)


@app.cell
def _(data, plt):
    from particle import Particle
    from matplotlib import colors
    import numpy as np


    def plot_top_particles_by_edep():
        rows, cols = 4, 3

        top_particles = data.groupby("PDG")["Edep"].sum().sort_values(ascending=False).head(rows*cols).index

        fig, axes = plt.subplots(rows, cols, figsize=(6 * cols, 6 * rows + 6))
        axes = axes.flatten()

        for i, pdg in enumerate(top_particles):
            p = Particle.from_pdgid(pdg)
            print(i, p.name)

            ax = axes[i]

            p_data = data[data["PDG"] == pdg]
            ax.hist2d(p_data["X"], p_data["Y"], weights=p_data["Edep"], norm=colors.LogNorm(), bins=100, cmap="magma", cmin=1)
            ax.set_title(p.name)
            ax.set_box_aspect(1)

        fig.suptitle("Energy deposit", fontsize=16, fontweight='bold')
        fig.tight_layout()
        plt.show()

    plot_top_particles_by_edep()
    return Particle, colors


@app.cell
def _(Particle, colors, data, plt):
    def plot_top_particles_by_count():
        rows, cols = 4, 3

        top_particles = data["PDG"].value_counts(ascending=False).head(rows*cols).index

        fig, axes = plt.subplots(rows, cols, figsize=(6 * cols, 6 * rows + 6))
        axes = axes.flatten()

        for i, pdg in enumerate(top_particles):
            p = Particle.from_pdgid(pdg)
            print(i, p.name)

            ax = axes[i]

            p_data = data[data["PDG"] == pdg]
            ax.hist2d(p_data["X"], p_data["Y"], norm=colors.LogNorm(), bins=100, cmap="magma", cmin=1)
            ax.set_title(p.name)
            ax.set_box_aspect(1)

        fig.suptitle("Record count", fontsize=16, fontweight='bold')
        fig.tight_layout()
        return plt.show()
    plot_top_particles_by_count()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
