#pragma once

namespace InteruptHandlers
{
    struct InterruptFrame;

    __attribute__((interrupt)) void InvalidOP(struct InterruptFrame* frame);

    __attribute__((interrupt)) void DeviceNotDetected(struct InterruptFrame* frame);

    __attribute__((interrupt)) void DoubleFault(struct InterruptFrame* frame);

    __attribute__((interrupt)) void SegmentNotPresent(struct InterruptFrame* frame);

    __attribute__((interrupt)) void StackSegmentFault(struct InterruptFrame* frame);

    __attribute__((interrupt)) void GeneralProtectionFault(struct InterruptFrame* frame);

    __attribute__((interrupt)) void PageFault(struct InterruptFrame* frame);

    __attribute__((interrupt)) void Keyboard(struct InterruptFrame* frame);

}
