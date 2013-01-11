#!/bin/bash

g++ $@ `pkg-config --libs opencv`
