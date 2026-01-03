export function uid(prefix = "n"): string {
  // crypto.randomUUID exists in modern browsers
  return `${prefix}_${crypto.randomUUID()}`;
}

