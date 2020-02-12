<TeXmacs|1.99.12>

<style|generic>

<\body>
  <doc-data|<doc-title|Verification of DDSYS>|<doc-author|<\author-data|<author-name|Chao
  Pan>>
    \;
  </author-data>>>

  <section|SDOF>

  <subsection|Classic SDOF system>

  Equation of motion

  <\equation>
    m <wide|u|\<ddot\>>+c <wide|u|\<dot\>>+k u=-m a<rsub|g>
  </equation>

  Parameter definition

  <\equation*>
    \<omega\>=<sqrt|<frac|k|m>><application-space|2em>\<zeta\>=<frac|c|2<sqrt|m
    k>>
  </equation*>

  Parameter setting

  <\equation*>
    <tabular*|<tformat|<table|<row|<cell|m=1>|<cell|>|<cell|k=100>|<cell|>|<cell|c=0.4>>|<row|<cell|>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|>|<cell|>|<cell|\<omega\>=10>|<cell|>|<cell|\<zeta\>=0.02>>>>>
  </equation*>

  <subsubsection|Case I: resonant exitation>

  <\equation>
    a<rsub|g>=sin<around*|(|\<omega\> t|)>
  </equation>

  <\big-figure|<image|sdof_resonance.eps|372pt|192pt||>>
    Response of SDOF system under resonant exitation
  </big-figure>

  <subsubsection|Case II: seismic exitation>

  Time-history curve of the ground motion is shown below

  <\big-figure|<image|eq.eps|367pt|185pt||>>
    Ground motion
  </big-figure>

  <\big-figure|<image|sdof_seismic.eps|372pt|185pt||>>
    Seismic response
  </big-figure>

  <subsection|SDOF system with bilinear spring>

  Equation of motion

  <\equation>
    m <wide|u|\<ddot\>>+c <wide|u|\<dot\>>+f<around*|(|u|)>=-m a<rsub|g>
  </equation>

  Parameter setting

  <\equation*>
    <tabular*|<tformat|<table|<row|<cell|m=1>|<cell|>|<cell|c=0.4>|<cell|>|<cell|>>|<row|<cell|>|<cell|>|<cell|>|<cell|>|<cell|>>|<row|<cell|k<rsub|0>=100>|<cell|>|<cell|u<rsub|y>=0.01>|<cell|>|<cell|\<alpha\>=0.1>>>>>
  </equation*>

  <subsubsection|Case I: resonant exitation>

  <\big-figure|<image|sdof_resonance_nl.eps|370pt|193pt||>>
    \;
  </big-figure>

  <\big-figure|<image|sdof_resonance_nl_df.eps|204pt|192pt||>>
    \;
  </big-figure>

  <subsubsection|Case II: seismic exitation>

  <\big-figure|<image|sdof_seismic_nl.eps|370pt|193pt||>>
    \;
  </big-figure>

  <\big-figure|<image|sdof_seismic_nl_df.eps|204pt|192pt||>>
    \;
  </big-figure>

  <\big-figure|<image|../disp_frame_damped.pdf|417pt|158pt||>>
    \;
  </big-figure>
</body>

<\initial>
  <\collection>
    <associate|font|roman>
    <associate|font-family|rm>
    <associate|math-font|roman>
  </collection>
</initial>

<\references>
  <\collection>
    <associate|auto-1|<tuple|1|1>>
    <associate|auto-10|<tuple|4|3>>
    <associate|auto-11|<tuple|5|3>>
    <associate|auto-12|<tuple|1.2.2|4>>
    <associate|auto-13|<tuple|6|4>>
    <associate|auto-14|<tuple|7|4>>
    <associate|auto-15|<tuple|8|?>>
    <associate|auto-2|<tuple|1.1|1>>
    <associate|auto-3|<tuple|1.1.1|1>>
    <associate|auto-4|<tuple|1|1>>
    <associate|auto-5|<tuple|1.1.2|1>>
    <associate|auto-6|<tuple|2|2>>
    <associate|auto-7|<tuple|3|2>>
    <associate|auto-8|<tuple|1.2|2>>
    <associate|auto-9|<tuple|1.2.1|3>>
  </collection>
</references>

<\auxiliary>
  <\collection>
    <\associate|figure>
      <tuple|normal|<\surround|<hidden-binding|<tuple>|1>|>
        Response of SDOF system under resonant exitation
      </surround>|<pageref|auto-4>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|2>|>
        Ground motion
      </surround>|<pageref|auto-6>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|3>|>
        Seismic response
      </surround>|<pageref|auto-7>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|4>|>
        \;
      </surround>|<pageref|auto-10>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|5>|>
        \;
      </surround>|<pageref|auto-11>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|6>|>
        \;
      </surround>|<pageref|auto-13>>

      <tuple|normal|<\surround|<hidden-binding|<tuple>|7>|>
        \;
      </surround>|<pageref|auto-14>>
    </associate>
    <\associate|toc>
      <vspace*|1fn><with|font-series|<quote|bold>|math-font-series|<quote|bold>|1<space|2spc>SDOF>
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-1><vspace|0.5fn>

      <with|par-left|<quote|1tab>|1.1<space|2spc>Classic SDOF system
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-2>>

      <with|par-left|<quote|2tab>|1.1.1<space|2spc>Case I: resonant exitation
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-3>>

      <with|par-left|<quote|2tab>|1.1.2<space|2spc>Case II: seismic exitation
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-5>>

      <with|par-left|<quote|1tab>|1.2<space|2spc>SDOF system with bilinear
      spring <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-8>>

      <with|par-left|<quote|2tab>|1.2.1<space|2spc>Case I: resonant exitation
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-9>>

      <with|par-left|<quote|2tab>|1.2.2<space|2spc>Case II: seismic exitation
      <datoms|<macro|x|<repeat|<arg|x>|<with|font-series|medium|<with|font-size|1|<space|0.2fn>.<space|0.2fn>>>>>|<htab|5mm>>
      <no-break><pageref|auto-12>>
    </associate>
  </collection>
</auxiliary>