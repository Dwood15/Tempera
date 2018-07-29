---
layout: post
title: Make Clion Fast
---

CLion is an editor based on the Jetbrains IntelliJ IDEA IDE.

I personally think it's worth checking out. If you're a student or you actively contribute to or represent an Open Source project, you can get licenses for free.

Unfortunately, CLion's performance is absolutely atrocious for large C/Cpp projects. My repository, Tempera, will freeze for seconds at a time, and I won't be able to interact with the ui for 
seconds at a time, either. I came up with two solutions.

For the first solution: ensure your machine has at least 4 cores. I'm running CLion on a quad-core i5. 

I've been doing some experimenting, and I've come up with a few configuration settings. In Clion, open Help -> Edit Custom Vm Options. save the file somewhere as a backup just in case.

I'm not going to explain it, as there's plenty of resources out there to explain CLion/JVM's vmoptions file.

```
-Xss1m
-Xms3g
-Xmx3g
-XX:NewSize=1g
-XX:MaxNewSize=1g
-XX:ReservedCodeCacheSize=512m
-XX:SoftRefLRUPolicyMSPerMB=50

-XX:+UseConcMarkSweepGC
-XX:+UseParNewGC
-XX:+UseBiasedLocking
-XX:+UseCMSInitiatingOccupancyOnly

-XX:+CMSClassUnloadingEnabled
-XX:CMSInitiatingOccupancyFraction=70
-XX:+CMSScavengeBeforeRemark
-XX:+CMSParallelRemarkEnabled

-XX:ParallelGCThreads=8
-XX:MaxTenuringThreshold=15
-XX:SurvivorRatio=12

-XX:+AggressiveOpts

-XX:MaxJavaStackTraceDepth=0
-XX:-OmitStackTraceInFastThrow

-XX:+ExplicitGCInvokesConcurrent
-XX:+UseStringCache

-XX:+UseFastAccessorMethods
-XX:+CMSIncrementalPacing
-XX:CompileThreshold=7000

-ea
-Dsun.io.useCanonCaches=true
-Dsun.io.useCanonPrefixCache=true
```

UseCanonCaches and UseCanonPrefixCache settings tell your JVM it can cache the compiled code. If you have it =false, your IDE will much slower.

Here's some optimizations I haven't added/gotten around to testing:

```
-Djava.net.preferIPv4Stack=true
-Djdk.http.auth.tunneling.disabledSchemes=""

-XX:+UseCompressedStrings
-XX:PermSize=512m
-XX:MaxPermSize=1g

-XX:-StackTraceInThrowable

-XX:+UseCompressedOops
-XX:MaxInlineSize=140
-XX:+OptimizeStringConcat
-agentlib:yjpagent64=probe_disable=*,disablealloc,disabletracing,onlylocal,disableexceptiontelemetry,delay=10000,sessionname=CLion2018.2

```
