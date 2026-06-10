# Claude Models — Alex's Field Guide

A plain-English breakdown of every current Claude model, what it costs, what it's
good at, and where it fits in your actual stack.

---

## Quick Reference Table

| Model | ID | Context | Max Output | Input $/1M | Output $/1M |
|---|---|---|---|---|---|
| **Fable 5** | `claude-fable-5` | 1M tokens | 128K | $10 | $50 |
| **Opus 4.8** | `claude-opus-4-8` | 1M tokens | 128K | $5 | $25 |
| **Opus 4.7** | `claude-opus-4-7` | 1M tokens | 128K | $5 | $25 |
| **Sonnet 4.6** | `claude-sonnet-4-6` | 1M tokens | 64K | $3 | $15 |
| **Haiku 4.5** | `claude-haiku-4-5` | 200K tokens | 64K | $1 | $5 |

> **Rule of thumb on pricing:** A typical back-and-forth message is ~1,000 tokens.
> $1/1M tokens = $0.001 per message. Haiku costs 1/10th of Sonnet, which costs
> 1/5th of Fable 5.

---

## The Models, One by One

---

### Fable 5 — `claude-fable-5`
**The ceiling. Use when you need the best and cost isn't the constraint.**

The most powerful model Anthropic has released. A tier above Opus entirely.
Adaptive thinking only — it reasons deeply on hard problems without you telling
it to.

**Capabilities:**
- Best long-horizon agentic work (multi-step, multi-file, overnight runs)
- Best at novel reasoning, strategy, and synthesis
- 1M context window — can hold your entire LifeOS repo in one conversation
- Streaming required for outputs above ~16K tokens

**What breaks from older models:**
- No `temperature`, `top_p`, or `top_k` — use prompting to steer instead
- No `budget_tokens` — use `thinking: {type: "adaptive"}` instead
- `thinking: {type: "disabled"}` returns a 400 — just omit the param
- No assistant prefills — use structured outputs instead

**Use it for:**
- Designing BEEP's full backend architecture (Auth0 + Express + Postgres schema)
- Writing the most complex game mechanics (battle resolution, training algorithms)
- When you want Silverback-quality reasoning for bots that actually debate
- Deep GDD or business planning sessions where you want pushback and strategy
- The 6-month roadmap refinement — acquisition pitch, valuation modeling

**Skip it when:** you're doing repetitive generation, quick lookups, or anything
where Sonnet or Opus already works. At $50/1M output tokens, a long session costs
real money.

---

### Opus 4.8 — `claude-opus-4-8`
**The everyday workhorse for serious engineering. This is Claude Code's default.**

Highly capable, autonomous, and good at multi-step tasks. Same API surface as
Opus 4.7 — no new breaking changes, just better reasoning, warmer writing, and
stronger long-horizon coherence.

**Capabilities:**
- State-of-the-art long-horizon agentic coding
- Strong at knowledge work, file-system memory, and multi-turn agents
- 1M context window at standard pricing (no long-context premium)
- Adaptive thinking enabled by default in Claude Code (`effort: xhigh`)

**Behavioral notes (vs 4.7):**
- More verbose narration between tool calls — add a silence instruction if you
  want terse output
- More deliberate — will ask before making minor choices (add autonomy
  instructions to cut ask-rate)
- Warmer, clearer prose than 4.7
- Reaches for tools (search, subagents) more conservatively — give explicit
  trigger conditions in tool descriptions

**Use it for:**
- All Claude Code sessions (it's already what you're running)
- Building BEEP's Node.js/Express backend (Phase 1)
- Debugging complex state issues in `beep-game.html`
- CPPd1 work — UE5 integration, complex C++ architecture questions
- Refactoring LifeOS pages for consistency
- Job application writing where quality matters

**Skip it when:** you're doing repetitive tasks at scale, quick data lookups,
or anything that Sonnet handles fine. Sonnet is 40% cheaper at similar speeds.

---

### Opus 4.7 — `claude-opus-4-7`
**Previous-gen Opus. Still excellent, slightly different personality.**

Same capability tier as 4.8 but:
- More terse and direct (less narration, less warm)
- More literal instruction following
- Design outputs use a "house style" (cream backgrounds, serif fonts) by default

You probably won't choose this directly unless you're testing 4.8 regressions or
pinning to a specific version for reproducibility.

**Use it when:** you pinned a session to this version and need to continue it,
or you're A/B testing 4.8 vs 4.7 on specific tasks.

---

### Sonnet 4.6 — `claude-sonnet-4-6`
**Best speed-to-intelligence ratio. The model to reach for when Opus is overkill.**

Fast, smart, and significantly cheaper than Opus. Supports adaptive thinking.
1M context window. 64K max output.

**Capabilities:**
- Great for structured tasks with clear scope
- Strong at code generation, editing, and review
- Computer use at high effort + adaptive thinking
- Streaming required for outputs above ~16K

**Use it for:**
- Generating boilerplate for LifeOS subpages (new expense category, new job entry)
- Quick JSON data file edits (`beep-bots.json`, `jobs.json`, `expenses.json`)
- Writing first drafts of blog posts or journal entries
- Compile scripts, build tooling, `package.json` updates
- Iterating on CSS/design-system changes
- BEEP shop item descriptions and flavor text
- Interview prep questions, cover letter drafts

**Skip it when:** you need deep reasoning, novel architecture, or you're
working on something with high stakes where Opus's judgment matters.

---

### Haiku 4.5 — `claude-haiku-4-5`
**Fastest, cheapest. For high-volume or time-sensitive tasks.**

The workhorse for tasks where speed and cost matter more than depth.
200K context (smaller than the others), 64K output. $1/$5 per million tokens.

**Capabilities:**
- Extremely fast latency
- Fine at classification, extraction, and structured generation
- Good for real-time features (autocomplete, quick lookups)
- 200K context is still large — fits most single-file codebases

**Use it for:**
- Classifying BEEP bot personalities or move types at scale
- Extracting structured data from expenses or job notes
- Quick yes/no judgments ("is this a valid expense category?")
- Generating lots of BEEP cosmetics flavor text or bot names
- Batch operations on many JSON records
- Any feature in a future BEEP backend where latency matters (bot validation,
  session health checks)
- Leaderboard calculations or scoring at scale

**Skip it when:** the task requires judgment, creativity, or multi-step reasoning.
Haiku follows instructions but doesn't improvise well.

---

## Decision Tree for Your Work

```
What am I doing?
│
├── Architecture / strategy / hard design decisions
│   └── Fable 5 (if budget allows) or Opus 4.8
│
├── Writing real code — backend, game mechanics, complex refactors
│   └── Opus 4.8 (already in Claude Code)
│
├── Writing code with clear scope — a new page, a bug fix, a small feature
│   └── Sonnet 4.6
│
├── Editing JSON data files, quick page tweaks, boilerplate
│   └── Sonnet 4.6 or Haiku 4.5
│
├── Batch generation — many items, repetitive structure
│   └── Haiku 4.5
│
├── Writing / long-form prose
│   ├── First draft, iterating → Sonnet 4.6
│   └── Final polish, pitch doc, acquisition deck → Opus 4.8 or Fable 5
│
└── In Claude Code (this terminal)
    └── Already Opus 4.8 at effort:xhigh — no change needed
```

---

## Model Features Cheat Sheet

| Feature | Fable 5 | Opus 4.8 | Opus 4.7 | Sonnet 4.6 | Haiku 4.5 |
|---|:---:|:---:|:---:|:---:|:---:|
| Adaptive thinking | ✅ | ✅ | ✅ | ✅ | ❌ |
| Extended thinking (budget_tokens) | ❌ | ❌ | ❌ | ✅ (deprecated) | ❌ |
| `effort` param (`xhigh`) | ✅ | ✅ | ✅ | ❌ | ❌ |
| `effort: max` | ✅ | ✅ | ✅ | ✅ | ❌ |
| 1M context | ✅ | ✅ | ✅ | ✅ | ❌ |
| 128K output | ✅ | ✅ | ✅ | ❌ | ❌ |
| Vision (images) | ✅ | ✅ | ✅ | ✅ | ✅ |
| Structured outputs | ✅ | ✅ | ✅ | ✅ | ✅ |
| Computer use | ✅ | ✅ | ✅ | ✅ | ❌ |
| Managed Agents | ✅ | ✅ | ✅ | ✅ | ✅ |
| temperature / top_p | ❌ | ❌ | ❌ | ✅ | ✅ |

---

## BEEP-Specific Guidance

**Phase 1 — Backend build:**
Use Opus 4.8 (Claude Code). The backend is complex enough that Sonnet will miss
edge cases in the auth flow, database schema, and WebSocket handling.

**Phase 2 — Silverback integration:**
Fable 5 or Opus 4.8. Real LLM reasoning for bots is the core value prop —
don't prototype with Haiku here even for speed.

**Phase 3 — Market data + multiplayer:**
Sonnet 4.6 for boilerplate matchmaking code. Opus 4.8 for the ranking algorithm
and fairness logic.

**Phase 4 — Cosmetics shop:**
Haiku 4.5 is fine for generating flavor text, bot descriptions, and skin names
at scale. Sonnet for anything customer-facing (UI copy, purchase confirmation).

---

## BEEP API Cost Estimate (rough)

If BEEP serves 1,000 DAU with ~5 bot battles/day each:

| Use case | Model | Est. tokens/op | Daily cost |
|---|---|---|---|
| Bot battle narration | Sonnet 4.6 | ~2K in + 500 out | ~$0.038 |
| Bot strategy (Silverback) | Opus 4.8 | ~3K in + 1K out | ~$0.040 |
| Leaderboard update | Haiku 4.5 | ~500 in + 100 out | ~$0.001 |
| User session validation | Haiku 4.5 | ~200 in + 50 out | <$0.001 |

At 1K DAU: ~$40-80/day in inference. At 10K DAU: ~$400-800/day.
This is why Phase 4 monetization timing matters — you need revenue before
inference costs scale.

---

## One-Line Summary

| Model | One line |
|---|---|
| **Fable 5** | The best Claude has. Use when you need maximum intelligence. |
| **Opus 4.8** | Your daily driver for real engineering and creative work. |
| **Opus 4.7** | Same tier as 4.8, slightly different personality. Skip unless pinned. |
| **Sonnet 4.6** | Fast, smart, affordable. The right tool for most coding tasks. |
| **Haiku 4.5** | Cheap and fast. Batch operations, real-time features, repetitive tasks. |
