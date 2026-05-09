import marimo

__generated_with = "0.23.5"
app = marimo.App(width="medium")


@app.cell
def _():
    import marimo as mo
    import uproot
    import pandas as pd
    import numpy as np

    return (pd,)


@app.cell
def _():
    import uproot
    import matplotlib.pyplot as plt
    import pandas as pd
    from collections import Counter

    file_path = "data/detector_simulation.root"

    pdg_counts = Counter()


    for chunk in uproot.iterate(f"{file_path}:Steps", filter_names=["PDG"], step_size="500MB"):
        counts = pd.Series(chunk["PDG"]).value_counts()
        for pdg, count in counts.items():
            pdg_counts[pdg] += count

    pdg_counts
    return pd, pdg_counts, plt


@app.cell
def _(pd, pdg_counts, plt):
    from particle import Particle

    top_pdgs = pd.Series(pdg_counts).sort_values(ascending=False)

    labels = [Particle.from_pdgid(i).name for i in top_pdgs.index]

    plt.figure(figsize=(10, 16))
    plt.barh(labels, top_pdgs.values)
    plt.xscale('log')
    plt.ylabel("Particle")
    plt.xlabel("Number of hits")
    plt.title("Hit count per particle")
    plt.show()
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
