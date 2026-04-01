#import "./Template/template.typ": *

#import "@preview/codelst:2.0.2": sourcecode
#import "@preview/acrostiche:0.5.1": *
#import "@preview/wordometer:0.1.4": total-words, word-count
#import "@preview/subpar:0.2.1"
#import "@preview/numbly:0.1.0": numbly

#set text(size: 10pt, region: "GB")
#set page(margin: 2.5cm)
#set list(indent: 2em)

// Acronym List
#init-acronyms((
  "IIR": "Infinite Impulse Response",
  "FIR": "Finite Impulse Response",
  "FFT": "Fast Fourier Transform",
  "SIMD": "Single Instruction Multiple Data",
  "MSE": "Mean Squared Error",
  "MAC": "Multiply-Accumulate",
  "CCS": "Code Composer Studio",
  "DSP": "Digital Signal Processing",
  "LP": "Low-Pass",
  "BP": "Band-Pass",
  "HP": "High-Pass",
  "SOS": "Second-Order Sections",
))

#show: strathy.with(
  // Make this shorterer
  title: "EE580 Mini Project",

  //takes in a list of dicts (name: full_name, reg: registration_number)
  authors: (
    (name: "Daniel Stebbings", reg: "202118874"),
    (name: "Preben Rasamanickam", reg: "202114642"),
  ),

  declaration: [
    We confirm and declare that this report and the project work is entirely the product of our own efforts and we have not used or presented the work of others herein without acknowledgement
  ],

  //abstract: [

  //],

  subtitle: [
    Department of Electronic and Electrical Engineering \
    University of Strathclyde, Glasgow
  ],

  // date: [your custom date]
  //default is datetime.today().display("[day] [month repr:long] [year]")

  // whether to gen a list of figs
  figures: false,

  // special space for the glossary, if using a glossary manager
  //glossary: [#print-index(title: "Definitions", sorted: "up")],

  // whether to generate the typst ack at the bottom
  ack: false,

  // compact layeout for assignments, set to false for more "grandeur"
  compact: true,
)
// table titles at the top if you like it that way
#show figure.where(
  kind: table,
): set figure.caption(position: top)


//The report is required to be in PDF format, and it should have the following structure:
//• Introduction
//• Part 1
//• Part 2
//• Part 3
//• Conclusion
//o Figures and tables (appropriately labelled)
//o Code (compulsory, in text format, and exhaustively commented)
//o References
//o Contributions
//A maximum of 5 pages is allowed for Introduction, Part1-3 and Conclusion sections
//combined; pages in excess will not be assessed. There is no page limit for figures, tables,
//code, references and contributions. A4 format, typical margins (~2.50cm) and font size
//(10-12pt) should be used.

= Introduction

This project involved the design and implementation of a real-time embedded #acr("DSP") system using the TMS320C6748 #acr("DSP") and OMAP-L138 Experimenter board. The primary objective was to acquire, store, and process audio signals through a user-controlled interface in real-time.

The system architecture can be divided into three functional states based on the DIP switch configuration:
- Standby: The system remains idle.
- Record: The system captures the latest 4 seconds of audio from the LINE IN into a circular buffer while providing a loopback to the LINE OUT.
- Playback: The system plays back the recorded 4-second buffer in a circular fashion.

During playback, the user can apply three distinct #acr("IIR") filters to the buffered audio - A #acr("LP"), #acr("BP"), and #acr("HP"), applied individually or in combination. The filters were designed to meet the following frequency specifications based on a sampling frequency of $f_s$ = 8 kHz:
- #acr("LP"): Cut-off at $f_1 ​= f_s/6$
- #acr("BP"): Pass-band between $f_1$​ and $f_2 ​= f_s/3$
- #acr("HP"): Cut-off at $f_2 ​= f_s/2$

Multiple optimisation methods were investigated, including #acr("SIMD") instructions, pragmas, and loop unrolling, and profiling was conducted to evaluate the cycle-count efficiency of the optimised implementation compared to the baseline version.

= Methodology

== MATLAB Filter Design and Validation

The initial phase involved designing three stable Infinite Impulse Response (IIR) filters using MATLAB Filter Designer to meet the previously specific spectral requirements.

== Embedded System Implementation

The system was developed in #acr("CCS") for the TMS320C6748 DSP amd is implemented on the OMAP-L138 Experimenter board.
The software architecture uses DSP/BIOS to manage real-time scheduling and hardware/software interrupts. The implementation is split between audio processing and lower-priority state management to ensure the 8kHz sampling rate is maintained for real-time performance.

=== State Machine

The system's behavior is controlled by a state machine that transitions between three primary states based on the status of DIP switches S2:1 and S2:2. This is implemented via two main functional blocks:

- *State Transition Logic* (dipState): A periodic function executing every 50ms to poll the DIP switches and update the global state variable g_sys_state. The system can have the following states:
  - *Standby*: Triggered when S2:1 is off. The system ignores other switches, turns off LEDs, and ensures no audio is output.
  - *Record*: Triggered when S2:1 is on and S2:2 is off. The system initialises the circular buffer, and records and outputs live audio.
  - *Playback*: Triggered when both S2:1 and S2:2 are on. If the circular buffer is full, the system transitions to processing the stored audio based on the filter selection. After processing, the system outputs the processed audio data in a circular fashion.

- *Real-Time Audio Handling* (audioHWI): This Hardware Interrupt Service Routine (ISR) executes at the sampling frequency. It handles the audio input/output of the system, performing the following based on g_sys_state:
  - *Standby*: A zero-valued sample is written to the codec.
  - *Record*: The system enqueues the incoming sample into the circular buffer and simultaneously routes it to the LINE OUT for loopback.
  - *Playback*: The next sample is fetched from the processed audio output buffer (g_audio_out) and writes it to the codec.

=== Buffer and Memory Management

To store 4 seconds of audio at 8kHz, a circular buffer structure was developed to manage 32000 16-bit samples.
- Circular Buffering: During the Record state, the buffer always containing the most recent 4 seconds of audio.
- When the buffer is full, newest sample is added at the tail position, the tail and head position are advanced   simultaneously, which overwrites the oldest sample.
- This implementation avoids memory shifts which have a high hardware cost.

To optimize memory usage, the processed audio output is produced by only processing smaller chunks of the circular buffer at a time. As a result, much smaller intermediary processing buffers can be used. This adds a small processing delay, which can be addressed by optimisation measures such as #acr("SIMD") intrinsics and loop unrolling.

=== Filter Integration
During Playback, the system polls DIP switches S2:6-8 to identify which filters (#acr("LP"), #acr("BP"), #acr("HP")) are active.
- Parallel Summation: If multiple filters are selected, the input chunk is passed through each active filter independently. The resulting outputs are summed together before being stored for playback.

== Optimisation and Profiling
To achieve maximum real-time performance, the baseline IIR implementation was refined into an optimised version. Several hardware-specific techniques were employed:
- Compiler Pragmas: \#pragma UNROLL and \#pragma MUST_ITERATE provided the compiler with loop count guarantees for software pipelining and loop unrolling.
- SIMD and Intrinsics: The following intrinsics were used for more efficient data operations:
  - amem4: Accessed 32-bit words (two 16-bit samples) simultaneously.
  - \_spint and \_spack2: Efficiently converted floating-point results back to integer format with saturation to prevent overflow.
  - \_sadd2: Performed addition on two packed 16-bit integers in a single cycle.

The performance impact of these optimisations was measured using the clock() function to capture the exact cycle counts for all seven filter permutations.

= Results

== Embedded System Filter Results

The frequency domain plot of the audio input buffer contents can be seen in @input_time_domain. The input signal has a broad spectral distribution of the sampled audio with a larger weighting of low-frequency components.

The result of applying the different permutations of filter configurations can be seen in the following figures:
- LP @output_lp_freq_domain
- BP: @output_bp_freq_domain
- HP: @output_hp_freq_domain
- LP+BP: @output_lp_bp_freq_domain
- LP+HP: @output_lp_hp_freq_domain
- BP+HP: @output_bp_hp_freq_domain
- LP+BP+HP: @output_all_freq_domain


== Embedded System Profiling Results

The profiling results of the IIR filter implementations are summarised in @profiling_results. The data compares the total CPU cycles required to process the 4-second audio buffer using the baseline filter application function versus the optimised version.

As shown in the table, the optimised implementation consistently improves the CPU runtime by approximately 34% across all active filter configurations. This performance gain is achieved primarily through loop unrolling and usage of #acr("SIMD") intrinsics.

= Discussion

The experimental data confirms that the OMAP-L138 Experimenter board is sufficiently capable of real-time audio processing when system features like hardware/software interrupts are leveraged.

The ~34% improvement in cycle counts highlights the effectiveness of several key techniques:
- Loop Unrolling and Pragmas: By using \#pragma UNROLL and \#pragma MUST_ITERATE, the compiler was able to schedule multiple instructions in parallel, taking advantage of the multiple execution units.
- #acr("SIMD") Intrinsics: The use of \_amem4 allowed for simultaneous loading of two 16-bit samples into a single 32-bit register, effectively doubling the memory throughput. While \_sadd2 and \_spack2 allowed for addition of four 16-bit samples in a single instruction, while providing hardware-level saturation.

The IIR filter implementation could be further optimised by leveraging fixed-point data types for faster arithmetic operations. For the highest level of optimisation possible, the filter implemntation could be written using linear assembly code.

= Conclusion

This project successfully demonstrated the design and implementation of a real-time embedded digital signal processing system using the TMS320C6748 DSP and OMAP-L138 Experimenter board. By integrating MATLAB-designed IIR filters with an optimised C implementation in #acr("CCS"), the system effectively managed audio acquisition, circular buffering, and frequency-selective playback at a sampling rate of 8 kHz.

Key findings and achievements from the project include:
- The implementation of a state machine allowed for seamless transitions between Standby, Record, and Playback modes via DIP switch user control.
- Use of compiler pragmas, circular buffers, and SIMD intrinsics significantly enhanced the processing efficiency.
- Profiling execution confirmed that hardware-specific optimisations reduced CPU cycle consumption by approximately 34% compared to the baseline implementation, ensuring the system comfortably met real-time processing constraints.

#pagebreak()

= Figures and Tables

== Embedded Systems

=== Audio Signals Results
#figure(
  image("Subsections/ccs_results/input_signal.dat.svg"),
  caption: "Input audio signal in frequency domain.",
) <input_time_domain>

#figure(
  image("Subsections/ccs_results/output_lp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with the LP filter.],
) <output_lp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_bp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with the BP filter.],
) <output_bp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_hp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with the HP filter.],
) <output_hp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_lp_bp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with combined LP and BP filters.],
) <output_lp_bp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_lp_hp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with combined LP and HP filters.],
) <output_lp_hp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_bp_hp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with combined BP and HP filters.],
) <output_bp_hp_freq_domain>

#figure(
  image("Subsections/ccs_results/output_lp_bp_hp.dat.svg"),
  caption: [Frequency domain plot of audio filtered with all three filters (LP+BP+HP) applied.],
) <output_all_freq_domain>

=== Profiling Results
#figure(
  table(
    columns: 6,
    align: left + horizon,
    stroke: 0.4pt,
    fill: (rgb("#f6f5f4"), rgb("#f6f5f4"), rgb("#f6f5f4"), none, none, none),
    table.cell(
      fill: rgb("#deddda"),
    )[HP],
    table.cell(
      fill: rgb("#deddda"),
    )[BP],
    table.cell(
      fill: rgb("#deddda"),
    )[LP],
    table.cell(
      fill: rgb("#deddda"),
    )[*Non-Opt Cycles*],
    table.cell(
      fill: rgb("#deddda"),
    )[*Opt Cycles*],
    table.cell(
      fill: rgb("#deddda"),
    )[*Ratio*],
    [0],
    [0],
    [0],
    [148527],
    [146389],
    [0.9856],
    [0],
    [0],
    [1],
    [53206075],
    [35400123],
    [0.6653],
    [0],
    [1],
    [0],
    [53165414],
    [35431354],
    [0.6664],
    [1],
    [0],
    [0],
    [53173689],
    [35407980],
    [0.6659],
    [0],
    [1],
    [1],
    [102647076],
    [67620590],
    [0.6588],
    [1],
    [0],
    [1],
    [102649226],
    [67595442],
    [0.6585],
    [1],
    [1],
    [0],
    [102654087],
    [67627048],
    [0.6588],
    [1],
    [1],
    [1],
    [152126303],
    [99803334],
    [0.6561],
  ),
  caption: "Performance comparison of IIR filter implementations",
) <profiling_results>


#pagebreak()

= Code Listings
== MATLAB Implementation
#include "./Subsections/MATLAB_code.typ"

== C Implementation <emb_impl>

#include "./Subsections/C_code.typ"


#pagebreak()

#show bibliography: set heading(numbering: "1")
#bibliography(
  (),
)
//#pagebreak()

= Contributions

Student, Daniel Stebbings:
- Responsible for the filter design, MATLAB code, and C IIR filter code.
- Contribution: 50%

Student, Preben Rasamanickam:
- Responsible for DSP/BIOS, buffering, state machine, and real-time processing.
- Contribution: 50%
