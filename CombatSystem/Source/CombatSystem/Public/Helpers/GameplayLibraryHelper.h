// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType,Meta = (Bitflags))
enum class ERelativeContext
{
	Behind,
	InFront,
	ToTheRight,
	ToTheLeft,
	Above,
	Below,
};
UENUM(BlueprintType, DisplayName = "RelativeContext")
enum class ERelativeContext_BP : uint8
{
	Behind,
	InFront,
	ToTheRight,
	ToTheLeft,
	Above,
	Below,
};
