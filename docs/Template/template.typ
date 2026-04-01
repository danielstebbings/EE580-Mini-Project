// The project function defines how your document looks.
// It takes your content and some metadata and formats it.
// Go ahead and customize it to your liking!
#let strathy(
  title: "",
  abstract: [],
  authors: (),
  glossary: [],
  date: datetime.today().display("[day] [month repr:long] [year]"),
  declaration: "I confirm and declare that this report and the assignment work is entirely the product of my own efforts and I have not used or presented the work of others herein.",
  subtitle: [
    Electronic & Electrical Engineering (MEng) \
    EE578: Advanced Digital Signal Processing \
    ],
  body,
  figures: true,
  listings: false,
  ack: true,
  compact: false,
  print: false
) = {
  // Set the document's basic properties.
  set document(author: authors.map(author => author.name), title: title)
  set page(
    number-align: center,
    // FIXME: Dunno, breaks the drafting package? Can reenable later -KP
    // margin: if print {(left: 3cm, right: 2cm, y: 2.5cm)}else{(x: 2.5cm, y: 2.5cm)}
  )
  set math.equation(numbering: "(1)")
  set text(lang: "en", size: 12pt)
  set align(center)
  // Title page.

  // strathclyde logo
  place(top+right, dy: -2.5cm, image("./strath.svg", width: 30%))
  // declaration
  if compact {
    v(20%)
    box(
      width: 65%,
      par(justify: true)[#text(size: 12pt, fill: gray, declaration)])
    v(30%)
  } else {
    v(40%)
  }
  //title
  text(2.5em, weight: 700, title)
  v(1.2em, weak: true)
  text(1.1em, date)
  v(1.2em, weak: true)
  //secondary info
  text(1.1em, subtitle)
  // Author information.
    grid(
      columns: (1fr,) * calc.min(3, authors.len()),
      gutter: 1em,
      ..authors.map(author => align(center, [#strong(author.name)\ reg. #author.reg]))
    )

  v(2.4fr)
  pagebreak()

  if not compact {
    v(40%)
    box(
      width: 75%,
      par(justify: true)[#text(size: 1.2em, declaration)])
    pagebreak()
  }


  // Main body.
  set align(left)
  set par(justify: true)
  set text(hyphenate: false)
  if abstract != [] {
    heading(outlined: false, numbering: none, text(0.85em, smallcaps[Abstract]))
    abstract
  }
  if not compact {
    pagebreak()
  }
  show outline.entry.where(
  level: 1
): set block(above: 1.5em)
  show outline.entry.where(
  level: 1
): set text(weight: "bold")
  outline(indent: auto)
  if figures {
    if not compact {pagebreak()}
    outline(
    title: [List of Figures],
    target: figure.where(kind: image))
  }
  if listings {
    pagebreak()
    outline(
    title: [Code Listings],
    target: figure.where(kind: raw))
  }
  if not compact {pagebreak()}
  glossary
  pagebreak()

  // Set the page numbering to start from 1 after the title and abstract.
  counter(page).update(1)
  set page(numbering: "1", number-align: center)
  set heading(numbering: "1.1")
  body

let LaTeX = {
  let A = (
    offset: (
      x: -0.33em,
      y: -0.3em,
    ),
    size: 0.7em,
  )
  let T = (
    x_offset: -0.12em    
  )
  let E = (
    x_offset: -0.2em,
    y_offset: 0.23em,
    size: 1em
  )
  let X = (
    x_offset: -0.1em
  )
  [L#h(A.offset.x)#text(size: A.size, baseline: A.offset.y)[A]#h(T.x_offset)T#h(E.x_offset)#text(size: E.size, baseline: E.y_offset)[E]#h(X.x_offset)X]
}


  // typst acknowledgment
  let acknowledgement = align(bottom + center, box(
  width: 70%,
  par(justify: true)[#text(size: 10pt)[This assignment was written using #link("https://typst.app")[#text(blue)[Typst]]. A modern alternative to #LaTeX written in Rust. This is a voluntary acknowledgment for the use of this tool. Source code used to generate the document including all the figures is available upon request]
]))

if ack {
  acknowledgement
}
}