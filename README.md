# ThruAudioXfer

[![unit-test](https://github.com/africamonkey/ThruAudioXfer/actions/workflows/unit-test.yml/badge.svg)](https://github.com/africamonkey/ThruAudioXfer/actions/workflows/unit-test.yml)

ThruAudioXfer (TAXf) is a protocol and application designed to let people download files through a speaker. It is a proof-of-concept created to demonstrate a long standing and poorly understood vulnerability in enterprise security.

The TAXf protocol is a transport protocol that allows one way transfer of data, between two peers, typically in the form of binary data bundles (files), though streams are possible. The protocol supports high latency, interrupted transfers and error detection. The TAXf transport protocol utilises audio waves as frames in an optical packet network, though it will work on any packet protocol. Quick Response Codes were chosen for the proof-of-concept because they were designed to be recognised and decoded rapidly by machine.

This idea comes from ThruGlassXfer (http://thruglassxfer.com/).

