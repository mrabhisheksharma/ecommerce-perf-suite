# E-Commerce Core Checkout Performance Test Suite

[![Test Tool: JMeter 5.6.3](https://img.shields.io/badge/JMeter-5.6.3-orange.svg)](https://jmeter.apache.org/)
[![LoadRunner: VuGen 2024](https://img.shields.io/badge/VuGen-2024-blue.svg)](https://www.microfocus.com/)
[![Git Branching: GitHub Flow](https://img.shields.io/badge/Branching-Feature_Branch_%2B_PR-brightgreen.svg)](#git-branching--collaboration-policy)
[![Jira Project: PERF](https://img.shields.io/badge/Jira-PERF-blue)](https://jira.company.internal/browse/PERF)

Welcome to the automated performance testing repository for the E-Commerce Core Checkout Platform. This project contains version-controlled load testing assets, workload models, test data generators, and summary reports.

---

## 1. Project Overview & Tested Journeys

This test suite validates system stability, throughput, and latency SLAs under peak holiday traffic scenarios (Black Friday 1,000 Concurrent Users / 800 TPS).

### Supported User Journeys:
1. **Homepage Browse (`01_Launch_Homepage`):** Anonymous guest arrival and health check.
2. **Catalog Search (`02_Search_Catalog`):** Keyword search query with pagination and filters.
3. **Cart Operations (`03_Add_To_Cart`):** Item addition and cart token correlation.
4. **Checkout & Payment (`04_Checkout_Payment`):** Payment gateway invocation and order placement (SLA: `< 2,000 ms`).

---

## 2. Tools & Environment Requirements

| Component | Tool / Technology | Version |
| :--- | :--- | :--- |
| **Primary Load Tool** | Apache JMeter | 5.6.3 (Java 17+ runtime required) |
| **Alternative Protocol Tool**| Micro Focus / OpenText VuGen | 2024 / 2023 R2 |
| **Version Control** | Git | 2.40+ |
| **Target Test Environment** | Staging-Perf-US-East | AWS EKS + PostgreSQL 16 RDS |
| **APM / Telemetry** | Grafana + Prometheus + Dynatrace | OpenTelemetry instrumented |

---

## 3. Repository Folder Structure

```text
ecommerce-perf-suite/
├── scripts/              # Performance test scripts (JMeter .jmx & VuGen folders)
│   ├── checkout_load_test.jmx  # Primary JMeter 5.6.3 test plan (XML)
│   └── vugen_checkout/         # LoadRunner VuGen script package
│       ├── vugen_checkout.usr  # VuGen settings & transactions
│       ├── vuser_init.c        # Pre-flight handshake & session registration
│       ├── Action.c            # Core user transaction loop & pacing
│       ├── vuser_end.c         # Session cleanup
│       └── default.cfg         # Run-time settings (think time, pacing)
├── data/                 # Seed test data in CSV format (NO real passwords!)
│   ├── users.csv         # 50,000 masked customer test accounts
│   └── products.csv      # Catalog inventory and SKUs
├── results/              # Directory for execution logs (excluded by .gitignore)
│   └── README.md         # Instructions on cloud artifact archival
├── reports/              # Version-controlled executive summary reports
│   └── Run_14_Peak_Load_Executive_Summary.md
├── .gitignore            # Excludes large JTLs, logs, temp caches, and secrets
└── README.md             # Project documentation (this file)
```

---

## 4. How to Execute Tests (CLI / Non-GUI Mode)

> **CRITICAL RULE:** Never run load tests using the JMeter graphical user interface (GUI). The GUI consumes excessive CPU/memory and skews latency measurements. Use CLI mode only!

### Standard 1,000 User Peak Load Test:
```bash
# From the root of the repository:
jmeter -n \
       -t scripts/checkout_load_test.jmx \
       -l results/run14_peak_1000vu.jtl \
       -j results/jmeter_run14.log \
       -e -o reports/html_run14/ \
       -Jthreads=1000 \
       -Jrampup=900 \
       -Jduration=2700 \
       -Jhost=staging-perf.store.internal
```

### Parameter Explanations:
- `-n`: Run in non-GUI (headless) mode.
- `-t`: Path to the JMX test script.
- `-l`: Path to output JTL file (contains raw sample metrics).
- `-j`: Path to JMeter execution log.
- `-e -o`: Generates interactive HTML dashboard report in target folder.
- `-J<key>=<value>`: Overrides runtime properties without editing script code.

---

## 5. What NOT to Commit (Git Policies)

Consult `.gitignore` for the complete rules. In summary:
1. ❌ **NEVER commit `.jtl` or raw result folders:** A single test run produces gigabytes of data. Archive results in AWS S3 or Jenkins artifacts.
2. ❌ **NEVER commit real credentials:** Use synthetic, pre-masked test data (`data/users.csv`).
3. ❌ **NEVER commit auto-generated HTML dashboards:** Generate reports on demand from JTL archives.

---

## 6. Git Branching & Collaboration Policy

To prevent merge conflicts in XML/C script files:
1. **One script owner at a time:** Coordinate on Jira before modifying shared `.jmx` or `.c` files.
2. **Branch naming convention:**
   - `feature/<jira-key>-<short-description>` (e.g., `feature/PERF-101-checkout-script`)
   - `fix/<jira-key>-<short-description>` (e.g., `fix/PERF-104-hikari-tuning`)
3. **Pull Request (PR) workflow:**
   - Always branch off latest `main`.
   - Never push directly to `main`.
   - Require 1 peer review approval before merging.
4. **Tag Every Important Test Run:**
   - Tag the exact commit that produced a baseline or milestone result:
     ```bash
     git tag -a run-14-baseline -m "Baseline run 14: 1000 VU peak load before HikariCP fix"
     git push origin run-14-baseline
     ```
   - This ensures 100% auditability and reproducibility months later.
