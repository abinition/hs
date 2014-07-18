>BS
! AeqSsp.bld - Build file for Site Specific Interfaces
>BE
>IS
! Standalone HyperScript without PROMIS
$ @aeq_ssp:build_hs.com                 # image

! PROMIS enabled HyperScript
$ @aeq_ssp:compile_hs.com

! Standalone HyperScript with PROMIS
$ @aeq_ssp:link_hs.com               # image

! AUTOMAN and General Automation Support Routines 
automan.for
automan2.for
automan3.for
automan4.for
autoutil.for
batch.for
>IE

