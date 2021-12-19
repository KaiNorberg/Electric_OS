#pragma once

namespace InteruptHandlers
{
    struct InterruptFrame;

    __attribute__((interrupt)) void InvalidOP(InterruptFrame* frame);

    __attribute__((interrupt)) void DeviceNotDetected(InterruptFrame* frame);

    __attribute__((interrupt)) void DoubleFault(InterruptFrame* frame);

    __attribute__((interrupt)) void SegmentNotPresent(InterruptFrame* frame);

    __attribute__((interrupt)) void StackSegmentFault(InterruptFrame* frame);

    __attribute__((interrupt)) void GeneralProtectionFault(InterruptFrame* frame);

    __attribute__((interrupt)) void PageFault(InterruptFrame* frame);

    __attribute__((interrupt)) void PIT(InterruptFrame* frame);

    __attribute__((interrupt)) void Keyboard(InterruptFrame* frame);

    __attribute__((interrupt)) void Mouse(InterruptFrame* frame);

    __attribute__((interrupt)) void SystemCall(InterruptFrame* frame);
}
