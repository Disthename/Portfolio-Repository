using Godot;
using System;

public class MeleeEnemy : BaseEnemy
{
	protected override void _on_EnemyBody_body_entered(Node body)
	{
		base._on_EnemyBody_body_entered(body);
		if (body == Player)
		{
			GD.Print("Attack Distance Reached");
			InAttackRange = true;
		}
		
	}

	protected override void _on_EnemyBody_body_exited(Node body)
	{
		if (body == Player)
		{
			InAttackRange = false;
		}
	}

	protected override void AttackPlayer()
	{
		EmitSignal(nameof(PlayerDamaged), AttackPower);
		GD.Print("Health Reduced");
		AttackCooldown.Start(TimeBetweenAttacks);
		AttackReady = false;
	}
	protected override void _on_Timer_timeout()
	{
		AttackReady = true;
	}
	

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		base._Ready();
	}

	// called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _PhysicsProcess(float delta)
	{
		base._PhysicsProcess(delta);
		if (AnimSprite.Animation == "default")
		{
			while (AttackReady && InAttackRange)
			{
				AttackPlayer();
			}
			if (PlayerFound)
			{
				LinearVelocity = Player.Position - this.Position;
				Position += LinearVelocity.Clamped(4.0f) * delta * MovementSpeed;
				this.LookAt(Player.Position);
			}
		}
	}
}




