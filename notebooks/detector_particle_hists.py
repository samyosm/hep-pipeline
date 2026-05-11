import marimo

__generated_with = "0.23.5"
app = marimo.App(width="medium")


@app.cell
def _():
    import ROOT

    ROOT.EnableImplicitMT()

    ROOT.gStyle.SetOptStat(0)
    ROOT.gStyle.SetPalette(ROOT.kBird)

    df = ROOT.RDataFrame("Steps", "data/detector_simulation.root")

    canvas = ROOT.TCanvas("c", "", 1800, 1200)
    canvas.SetLogz()

    canvas.Divide(3, 2)

    particles = {
        11: "e-",
        22: "gamma",
        13: "mu-",
        2112: "neutron",
        2212: "proton",
        211: "pi+"
    }

    histograms = []

    for i, (pdg, name) in enumerate(particles.items(), start=1):

        canvas.cd(i)

        ROOT.gPad.SetLogz()

        h = (
            df.Filter(f"PDG == {pdg}")
              .Histo2D(
                  (
                      f"h_{pdg}",
                      f"{name};X;Y",
                      250, -2000, 2000,
                      250, -2000, 2000
                  ),
                  "X",
                  "Y",
                  "Edep"
              )
        )

        h.SetMinimum(1e-6)

        h.Draw("COLZ")

        histograms.append(h)

    canvas.SaveAs("particles.png")
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
