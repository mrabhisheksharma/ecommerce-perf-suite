# Performance Test Executive Summary Report: Run 14

**Application:** E-Commerce Core Checkout Platform  
**Target Environment:** Staging-Perf-US-East  
**Build Version:** `v2.14.0-rc3` (Git commit: `a3f9c1e`, Tag: `run-14-baseline`)  
**Test Objective:** Black Friday Peak Load Validation (Target: 1,000 Virtual Users / 800 TPS)  
**Execution Window:** 2026-10-19 14:00:00 UTC – 15:00:00 UTC (15m Ramp, 45m Steady-State....)  
**Lead Performance Engineer:** Abhishek  
**Overall Verdict:** ❌ **CONDITIONAL FAIL (Critical Defect PERF-104 Logged)**

---

## 1. Executive Summary & Key Findings

1. **Homepage, Search, and Add to Cart Passed SLA:**  
   Browsing and catalog search transactions remained well within the SLA (< 2,000 ms), exhibiting sub-second 90th percentile response times even at 1,000 concurrent virtual users.
2. **Checkout Payment Transaction Failed SLA (8.42s vs 2.0s Target):**  
   The critical `04_Checkout_Payment` API suffered severe latency degradation beyond 640 concurrent users. At steady state, 90th percentile response times surged to **8,420 ms**, accompanied by a **6.22% HTTP 504 Gateway Timeout rate**.
3. **Root Cause Confirmed (Connection Pool Exhaustion):**  
   APM and database telemetry revealed HikariCP connection pool starvation. While PostgreSQL database CPU remained healthy at only **26%**, the application server's max connection pool was capped at **50 connections**. 184 worker threads queued for connections, hitting the 30-second timeout.
4. **Action Taken:**  
   Defect **PERF-104** (Severity: Critical, Priority: High) has been raised in Jira with complete telemetry evidence attached.

---

## 2. Quantitative Performance Metrics Table

| Transaction Name | Total Samples | 90th % Response Time | SLA Target | Status | Error Rate % | Throughput (TPS) |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **01_Launch_Homepage** | 48,000 | 450 ms | < 1,500 ms | ✔️ PASS | 0.00% | 133.3 /s |
| **02_Search_Catalog** | 36,000 | 720 ms | < 2,000 ms | ✔️ PASS | 0.00% | 100.0 /s |
| **03_Add_To_Cart** | 24,000 | 890 ms | < 2,000 ms | ✔️ PASS | 0.04% | 66.7 /s |
| **04_Checkout_Payment** | 18,200 | **8,420 ms** | **< 2,000 ms** | ❌ **FAIL** | **6.22%** | **50.5 /s** |
| **TOTAL WORKLOAD** | **126,200** | **1,840 ms** | **N/A** | ⚠️ **ALERT** | **0.90%** | **350.5 /s** |

---

## 3. Server Resource Utilization (Steady-State Peak)

| Tier / Host | Metric | Observed Value | Threshold | Status |
| :--- | :--- | :--- | :--- | :--- |
| **App Pods (Spring Boot)** | CPU Utilization | 64% | < 75% | Healthy |
| **App Pods (JVM Heap)** | G1GC Pause Time | Max 42 ms | < 200 ms | Healthy |
| **HikariCP Connection Pool** | Active Connections | **50 / 50 (100%)** | < 80% | ❌ **Exhausted** |
| **HikariCP Thread Queue** | Pending Threads | **184 waiting** | 0 | ❌ **Starved** |
| **PostgreSQL RDS (db.r6g.2xlarge)**| CPU Utilization | 26% | < 70% | Healthy / Idle |
| **PostgreSQL Disk IOPS** | Read/Write IOPS | 840 IOPS | 3,000 IOPS | Healthy |

---

## 4. Reproducibility & Git Traceability

To reproduce this exact test execution from version control:
```bash
# 1. Checkout baseline tag
git checkout tags/run-14-baseline

# 2. Run CLI execution command
jmeter -n -t scripts/checkout_load_test.jmx \
       -l results/run14_peak_1000vu.jtl \
       -j results/jmeter_run14.log \
       -e -o reports/html_run14/
```

- **Jira Defect Reference:** [PERF-104](https://jira.company.internal/browse/PERF-104)
- **APM Snapshot URI:** `https://grafana.internal/d/perf-payment-svc?from=1729346400000&to=1729350000000`
- **Next Milestone:** Retest scheduled for Build `v2.14.0-rc4` on 2026-10-21.
