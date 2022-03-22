#!/bin/bash
python -m pip install glad
glad --generator=c --spec gl --api="gl=4.1" --out-path=.
glad --generator=c --spec wgl --out-path=.
glad --generator=c --spec glx --out-path=.
