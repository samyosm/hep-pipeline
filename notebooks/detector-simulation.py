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
    import time
    from datetime import timedelta

    return ROOT, mo, np, plt, time, timedelta


@app.cell
def _():
    from particle import Particle

    relevant_particles = Particle.findall(
        lambda p: (getattr(p, "lifetime", 0) > 1e-20) 
    )

    particle_dict = {f"{p.name} ({int(p.pdgid)})": int(p.pdgid) for p in relevant_particles}
    return Particle, particle_dict


@app.cell
def _(mo, particle_dict):
    form = mo.md("{bins} {particles} {energy}").batch(
        bins=mo.ui.number(start=0, value=200, label="Bin count"),
        particles=mo.ui.multiselect(options=particle_dict, label="Particle PDG"),
        energy=mo.ui.checkbox(value=False, label="Energy deposit")
    ).form()
    form
    return (form,)


@app.cell
def _(form):
    bins = form.value["bins"]
    energy = form.value["energy"]
    particles_pdg = form.value["particles"]
    return bins, energy, particles_pdg


@app.cell
def _(ROOT, bins, energy, particle_dict, particles_pdg, time):
    ROOT.EnableImplicitMT()

    start_time = time.perf_counter()

    df = ROOT.RDataFrame("Steps", "detector_simulation.root")

    if (len(particles_pdg) == len(particle_dict)):
        df = df
    else:
        filter_expr = " || ".join([f"PDG == {pdg}" for pdg in particles_pdg])
        df = df.Filter(filter_expr)

    if (energy):
        h = df.Histo2D(("h", "", bins, -4000, 4000, bins, -4000, 4000),"X", "Y", "Edep")\
                .GetValue()
    else:
        h = df.Histo2D(("h", "", bins, -4000, 4000, bins, -4000, 4000),"X", "Y")\
                .GetValue()
    return h, start_time


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
def _(H, Particle, energy, mo, particle_dict, particles_pdg, plt):
    from matplotlib.colors import LogNorm

    plt.figure(figsize=(8,6), dpi=300)

    plt.imshow(
        H.T,
        aspect="equal",
        origin="lower",
        norm=LogNorm(),
        cmap="inferno",
        extent=[-4000, 4000, -4000, 4000]
    )

    plt.gca().minorticks_on()

    plt.grid(visible=True, which='major', color='#7f7f7f', linestyle=':', linewidth=0.3, alpha=0.2)
    plt.grid(visible=True, which='minor', color='#7f7f7f', linestyle=':', linewidth=0.3, alpha=0.2)

    plt.colorbar(label="Energy deposit (log scale)")
    plt.xlabel("X (mm)")
    plt.ylabel("Y (mm)")

    particle_names = [f"${Particle.from_pdgid(pdg).latex_name}$" for pdg in particles_pdg]

    plt.title(f"Detector {'energy' if energy else 'deposit'} map (XY, Particle(s): {'All' if len(particles_pdg) == len(particle_dict) else ', '.join(particle_names)})")

    plt.savefig(f"data/detector_{'energy' if energy else 'deposit'}_map_pdg_{'All' if len(particles_pdg) == len(particle_dict) else '-'.join([Particle.from_pdgid(pdg).programmatic_name for pdg in particles_pdg])}_tracker_ecal_hcal.png", dpi=1200)

    ax = mo.ui.matplotlib(plt.gca())
    ax
    return


@app.cell
def _(mo, start_time, time, timedelta):
    elapsed = time.perf_counter() - start_time

    mo.md(
      f"""
      Time taken: {timedelta(seconds=elapsed)}
      """
    )
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
