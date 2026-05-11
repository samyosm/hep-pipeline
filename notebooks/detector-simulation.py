import marimo

__generated_with = "0.23.5"
app = marimo.App(width="medium")


@app.cell
def _():
    import matplotlib.pyplot as plt
    import numpy as np
    import uproot
    import ROOT

    import marimo as mo

    return ROOT, mo, np, plt


@app.cell
def _(mo):
    form = mo.md("{bins} {particle_pdg}").batch(
        bins=mo.ui.number(start=0, value=200, label="Bin count"),
        particle_pdg=mo.ui.number(value=11, label="Particle PDG")
    ).form()
    form
    return (form,)


@app.cell
def _(form):
    bins = form.value["bins"]
    particle_pdg = form.value["particle_pdg"]
    return bins, particle_pdg


@app.cell
def _(ROOT, bins, particle_pdg):
    ROOT.EnableImplicitMT()

    df = ROOT.RDataFrame("Steps", "data/detector_simulation.root")

    h = df\
        .Filter(f"PDG == {particle_pdg}")\
        .Histo2D(("h", "", bins, -1500, 1500, bins, -1500, 1500),"X", "Y", "Edep")\
            .GetValue()
    return (h,)


@app.cell
def _(h, np):
    nx = h.GetNbinsX()
    ny = h.GetNbinsY()

    H = np.zeros((nx, ny))

    for i in range(nx):
        for j in range(ny):
            H[i, j] = h.GetBinContent(i+1, j+1)
    return (H,)


@app.cell
def _(H, mo, plt):
    from matplotlib.colors import LogNorm

    plt.figure(figsize=(8,6), dpi=300)

    plt.imshow(
        H.T,
        aspect="equal",
        origin="lower",
        norm=LogNorm(),
            cmap="inferno"
    )

    plt.colorbar(label="Energy deposit (log scale)")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.title("Detector energy map (XY, log scale)")

    ax = mo.ui.matplotlib(plt.gca())
    ax
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
