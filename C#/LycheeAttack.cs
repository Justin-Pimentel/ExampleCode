using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LycheeAttack : MonoBehaviour {
    //The current bullet used
    public GameObject projectile;
    //The position of the target (Player)
    public Transform to;
    //A var to keep track of time between attacks
    private float attackTimer;
    //How long it should wait to attack again
    public float cooldown;
    //This should always be set to -90 cuz it works. Change it to get weird rotations.
    public float offset;
    //Range the player must be in to get shot at
    public float range;

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        //How far the player is from the poor lychee
        float distFromPlayer = Vector3.Distance(transform.position, to.position);

		if(attackTimer <= 0 && distFromPlayer <= range)
        {
            //Get quaternion with correct angle information from monster to player
            Quaternion q = getAngle();
            //Debug.Log("Angle: " + angle);
            //Create projectile of type projectile, at current lychee position, with rotation info in quaternion q
            Instantiate(projectile, transform.position, q);
            //Reset timer
            attackTimer = cooldown;
        }
        else
        {
            //Countdown to next attack
            attackTimer -= Time.deltaTime;
        }
	}

    Quaternion getAngle()
    {
        //All of this is space magic. I cannot explain it.
        Vector3 dirVector = to.position - transform.position;
        float angleZ = Mathf.Atan2(dirVector.y, dirVector.x) * Mathf.Rad2Deg;
        Quaternion q = Quaternion.Euler(0f, 0f, angleZ + offset);

        return q;
    }
}
