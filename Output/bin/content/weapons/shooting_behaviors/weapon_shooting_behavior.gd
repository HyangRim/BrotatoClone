class_name WeaponShootingBehavior
extends Behavior

var __next_attack_id: = 1


func shoot(_distance: float) -> void :
	pass


func _get_next_attack_id() -> int:
	var id: = __next_attack_id
	__next_attack_id += 1
	return id
