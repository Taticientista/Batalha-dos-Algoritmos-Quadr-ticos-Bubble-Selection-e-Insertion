#!/usr/bin/env python3
"""Resume benchmark CSV; uses only Python standard library unless --plot is set."""
import argparse
import csv
import statistics
from collections import defaultdict
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("csv_file", help="CSV produzido pelo benchmark")
    parser.add_argument("--plot", action="store_true", help="salva gráficos PNG (requer matplotlib)")
    args = parser.parse_args()

    path = Path(args.csv_file)
    groups = defaultdict(list)
    with path.open(newline="", encoding="utf-8") as handle:
        reader = csv.DictReader(handle)
        required = {"scenario", "algorithm", "n", "comparisons", "movements", "seconds",
                    "sorted", "permutation_preserved"}
        if not required.issubset(reader.fieldnames or []):
            raise SystemExit(f"CSV não contém todas as colunas necessárias: {sorted(required)}")
        for row in reader:
            if row["sorted"] != "1" or row["permutation_preserved"] != "1":
                raise SystemExit(f"Resultado inválido encontrado: {row}")
            key = (row["scenario"], row["algorithm"], int(row["n"]))
            groups[key].append({
                "comparisons": int(row["comparisons"]),
                "movements": int(row["movements"]),
                "seconds": float(row["seconds"]),
            })

    print(f"Arquivo: {path}")
    print(f"{'Cenário':<12} {'Algoritmo':<10} {'n':>7} {'Comp. média':>15} "
          f"{'Mov. média':>15} {'Tempo mediano (s)':>20} {'Desvio (s)':>12}")
    for (scenario, algorithm, n), rows in sorted(groups.items()):
        comps = statistics.mean(r["comparisons"] for r in rows)
        moves = statistics.mean(r["movements"] for r in rows)
        times = [r["seconds"] for r in rows if r["seconds"] >= 0]
        median = statistics.median(times) if times else float("nan")
        deviation = statistics.pstdev(times) if len(times) > 1 else 0.0
        print(f"{scenario:<12} {algorithm:<10} {n:>7} {comps:>15.1f} "
              f"{moves:>15.1f} {median:>20.9f} {deviation:>12.9f}")

    if args.plot:
        try:
            import matplotlib.pyplot as plt
        except ImportError as exc:
            raise SystemExit("Instale matplotlib para gráficos: python3 -m pip install matplotlib") from exc
        output_dir = path.parent
        for metric, label in [
            ("comparisons", "Comparações médias"),
            ("movements", "Movimentações médias"),
            ("seconds", "Tempo mediano (s)")
        ]:
            fig, ax = plt.subplots()
            for scenario in sorted({key[0] for key in groups}):
                for algorithm in ["Bubble", "Selection", "Insertion"]:
                    points = []
                    for (sc, alg, n), rows in groups.items():
                        if sc == scenario and alg == algorithm:
                            vals = [r[metric] for r in rows]
                            value = statistics.median(vals) if metric == "seconds" else statistics.mean(vals)
                            points.append((n, value))
                    if points:
                        points.sort()
                        ax.plot([p[0] for p in points], [p[1] for p in points],
                                marker="o", label=f"{algorithm} / {scenario}")
            ax.set_xlabel("Tamanho do vetor (n)")
            ax.set_ylabel(label)
            ax.set_title(label)
            ax.grid(True, alpha=0.3)
            ax.legend(fontsize="small")
            fig.tight_layout()
            filename = output_dir / f"{metric}.png"
            fig.savefig(filename, dpi=160)
            plt.close(fig)
            print(f"Gráfico salvo: {filename}")


if __name__ == "__main__":
    main()
