/** @type {import('./$types').PageLoad} */
export async function load({ params}) {
	const res = await fetch(`/api/items/${params.id}`);
	// const item = await res.json();
	
	return { params };
	}
	// /** @type {import('./$types').PageData} */
    // export let data;